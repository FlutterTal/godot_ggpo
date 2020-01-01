#include "ggpo.h"
#include <ggponet.h>

#include <string>

GGPO* GGPO::singleton = NULL;

int player, count, total, frames_ahead, disconnect_timeout;

GGPO::GGPO() {
    singleton = this;
}

GGPO::~GGPO() {
    singleton = NULL;
}

GGPO* GGPO::get_singleton() {
    return GGPO::singleton;
}

void GGPO::start_session(const String& game, int numPlayers, int localPort) {
    GGPOSessionCallbacks cb;

    cb.begin_game = &Callbacks::beginGame;
    cb.advance_frame = &Callbacks::advanceFrame;
    cb.load_game_state = &Callbacks::loadGameState;
    cb.log_game_state = &Callbacks::logGameState;
    cb.save_game_state = &Callbacks::saveGameState;
    cb.free_buffer = &Callbacks::freeBuffer;
    cb.on_event = &Callbacks::onEvent;

    GGPOSession* ggpo;
    if(ggpo_start_session(&ggpo, &cb, game.utf8().get_data(), numPlayers, sizeof(uint64_t), localPort) != GGPO::OK) {
        return;
    }

    GGPO::set_ggpoptr(ggpo);
}

void GGPO::start_spectating(const String& game, int numPlayers, int localPort, const String& hostIp, int hostPort) {
    GGPOSessionCallbacks cb;

    cb.begin_game = &Callbacks::beginGame;
    cb.advance_frame = &Callbacks::advanceFrame;
    cb.load_game_state = &Callbacks::loadGameState;
    cb.log_game_state = &Callbacks::logGameState;
    cb.save_game_state = &Callbacks::saveGameState;
    cb.free_buffer = &Callbacks::freeBuffer;
    cb.on_event = &Callbacks::onEvent;

    GGPOSession* ggpo;
    if(ggpo_start_spectating(&ggpo, &cb, game.utf8().get_data(), numPlayers, sizeof(uint64_t), localPort, hostIp.utf8().ptrw(), hostPort) != GGPO::OK) {
        return;
    }
}

void GGPO::set_disconnect_notify_start(int timeout) {
    if(ggpo_set_disconnect_notify_start(GGPO::get_ggpoptr(), timeout) != GGPO::OK) {
        return;
    }
}

void GGPO::set_disconnect_timeout(int timeout) {
    if(ggpo_set_disconnect_timeout(GGPO::get_ggpoptr(), timeout) != GGPO::OK) {
        return;
    }
}

void GGPO::synchronize_input(int inputs, int length, int disconnectFlags) {
    if(ggpo_synchronize_input(GGPO::get_ggpoptr(), &inputs, sizeof(uint64_t) * length, &disconnectFlags) != GGPO::OK) {
        return;
    }
}

void GGPO::add_local_input(int localPlayerHandle, uint64_t input) {
    if(ggpo_add_local_input(GGPO::get_ggpoptr(), localPlayerHandle, &input, sizeof(uint64_t)) != GGPO::OK) {
        return;
    }
}

void GGPO::close_session() {
    if(ggpo_close_session(GGPO::get_ggpoptr()) != GGPO::OK) {
        return;
    }
}

void GGPO::idle(int timeout) {
    if(ggpo_idle(GGPO::get_ggpoptr(), timeout) != GGPO::OK) {
        return;
    }
}

void GGPO::add_player(int playerType, int playerNum, const String& playerIpAddress, int playerPort, int pHandle) {
    GGPOPlayer player;

    player.size = sizeof(GGPOPlayer);
    player.type = (GGPOPlayerType)playerType;
    player.player_num = playerNum;
    strcpy_s(player.u.remote.ip_address, playerIpAddress.utf8().get_data());
    player.u.remote.port = playerPort;

    if(ggpo_add_player(GGPO::get_ggpoptr(), &player, &pHandle) != GGPO::OK) {
        return;
    }
}

void GGPO::disconnect_player(int pHandle) {
    if(ggpo_disconnect_player(GGPO::get_ggpoptr(), pHandle) != GGPO::OK) {
        return;
    }
}

void GGPO::set_frame_delay(int pHandle, int frameDelay) {
    if(ggpo_set_frame_delay(GGPO::get_ggpoptr(), pHandle, frameDelay) != GGPO::OK) {
        return;
    }
}

void GGPO::advance_frame() {
    if(ggpo_advance_frame(GGPO::get_ggpoptr()) != GGPO::OK) {
        return;
    }
}

void GGPO::log(const String& text) {
    ggpo_log(GGPO::get_ggpoptr(), text.utf8().get_data());
}

Dictionary GGPO::get_network_stats(int pHandle) {
    GGPONetworkStats stats;
    Dictionary d;

    d["result"] = ggpo_get_network_stats(GGPO::get_ggpoptr(), pHandle, &stats);
    d["sendQueueLen"] = stats.network.send_queue_len;
    d["recvQueueLen"] = stats.network.recv_queue_len;
    d["ping"] = stats.network.ping;
    d["kbpsSent"] = stats.network.kbps_sent;
    d["localFramesBehind"] = stats.timesync.local_frames_behind;
    d["remoteFramesBehind"] = stats.timesync.remote_frames_behind;

    return d;
}

int fletcher32_checksum(short* data, size_t len) {
    int sum1 = 0xffff, sum2 = 0xffff;
    while(len) {
        size_t tlen = len > 360 ? 360 : len;
        len -= tlen;
        do {
            sum1 += *data++;
            sum2 += sum1;
        } while(--tlen);
        sum1 = (sum1 & 0xffff) + (sum1 >> 16);
        sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    }

    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    return sum2 << 16 | sum1;
}

bool Callbacks::beginGame(const char* game) {
    return true;
}

bool Callbacks::advanceFrame(int flags) {
    GGPO::get_singleton()->emit_signal("advance_frame");
    return true;
}

bool Callbacks::loadGameState(unsigned char* buffer, int length) {
    GGPO::get_singleton()->emit_signal("load_game_state", buffer, length);
    return true;
}

bool Callbacks::logGameState(char* filename, unsigned char* buffer, int length) {
    GGPO::get_singleton()->emit_signal("log_game_state", filename, buffer);
    return true;
}

bool Callbacks::saveGameState(unsigned char** buffer, int* len, int* checksum, int frame) {
    GGPO::get_singleton()->emit_signal("save_game_state", buffer, len, checksum);
    return true;
}

void Callbacks::freeBuffer(void* buffer) {
    free(buffer);
}

bool Callbacks::onEvent(GGPOEvent* info) {
    switch(info->code) {
        case GGPO::EVENTCODE_CONNECTED_TO_PEER:
            player = info->u.connected.player;
            GGPO::get_singleton()->emit_signal("event_connected_to_peer", player);
            break;
        case GGPO::EVENTCODE_SYNCHRONIZING_WITH_PEER:
            player = info->u.synchronizing.player;
            count = info->u.synchronizing.count;
            total = info->u.synchronizing.total;
            GGPO::get_singleton()->emit_signal("event_synchronizing_with_peer", player, count, total);
            break;
        case GGPO::EVENTCODE_SYNCHRONIZED_WITH_PEER:
            player = info->u.synchronized.player;
            GGPO::get_singleton()->emit_signal("event_synchronized_with_peer", player);
            break;
        case GGPO::EVENTCODE_RUNNING:
            GGPO::get_singleton()->emit_signal("event_running");
            break;
        case GGPO::EVENTCODE_DISCONNECTED_FROM_PEER:
            player = info->u.disconnected.player;
            GGPO::get_singleton()->emit_signal("event_disconnected_from_peer", player);
            break;
        case GGPO::EVENTCODE_TIMESYNC:
            frames_ahead = info->u.timesync.frames_ahead;
            GGPO::get_singleton()->emit_signal("event_timesync", frames_ahead);
            break;
        case GGPO::EVENTCODE_CONNECTION_INTERRUPTED:
            player = info->u.connection_interrupted.player;
            disconnect_timeout = info->u.connection_interrupted.disconnect_timeout;
            GGPO::get_singleton()->emit_signal("event_connection_interrupted", player, disconnect_timeout);
            break;
        case GGPO::EVENTCODE_CONNECTION_RESUMED:
            player = info->u.connection_resumed.player;
            GGPO::get_singleton()->emit_signal("event_connection_resumed", player);
            break;
    }

    return true;
}

void GGPO::_bind_methods() {
    ClassDB::bind_method(D_METHOD("start_session", "game", "numPlayers", "localPort"), &GGPO::start_session);
    ClassDB::bind_method(D_METHOD("start_spectating", "game", "numPlayers", "localPort", "hostIp", "hostPort"), &GGPO::start_spectating);
    ClassDB::bind_method(D_METHOD("set_disconnect_notify_start", "timeout"), &GGPO::set_disconnect_notify_start);
    ClassDB::bind_method(D_METHOD("set_disconnect_timeout", "timeout"), &GGPO::set_disconnect_timeout);
    ClassDB::bind_method(D_METHOD("synchronize_input", "inputs", "length", "disconnectFlags"), &GGPO::synchronize_input);
    ClassDB::bind_method(D_METHOD("add_local_input", "localPlayerHandle", "input"), &GGPO::add_local_input);
    ClassDB::bind_method(D_METHOD("close_session"), &GGPO::close_session);
    ClassDB::bind_method(D_METHOD("idle", "timeout"), &GGPO::idle);
    ClassDB::bind_method(D_METHOD("add_player", "playerType", "playerNum", "playerIpAddress", "playerPort", "pHandle"), &GGPO::add_player);
    ClassDB::bind_method(D_METHOD("disconnect_player", "pHandle"), &GGPO::disconnect_player);
    ClassDB::bind_method(D_METHOD("set_frame_delay", "pHandle", "frameDelay"), &GGPO::set_frame_delay);
    ClassDB::bind_method(D_METHOD("advance_frame"), &GGPO::advance_frame);
    ClassDB::bind_method(D_METHOD("log", "text"), &GGPO::log);
    ClassDB::bind_method(D_METHOD("get_network_stats", "pHandle"), &GGPO::get_network_stats);

    ADD_SIGNAL(MethodInfo("advance_frame"));
    ADD_SIGNAL(MethodInfo("load_game_state", PropertyInfo(Variant::OBJECT, "buffer"), PropertyInfo(Variant::INT, "length")));
    ADD_SIGNAL(MethodInfo("log_game_state", PropertyInfo(Variant::STRING, "filename"), PropertyInfo(Variant::OBJECT, "buffer")));
    ADD_SIGNAL(MethodInfo("save_game_state", PropertyInfo(Variant::OBJECT, "buffer"), PropertyInfo(Variant::INT, "len"), PropertyInfo(Variant::INT, "checksum")));
    ADD_SIGNAL(MethodInfo("event_connected_to_peer", PropertyInfo(Variant::INT, "player")));
    ADD_SIGNAL(MethodInfo("event_synchronizing_with_peer", PropertyInfo(Variant::INT, "player"), PropertyInfo(Variant::INT, "count"), PropertyInfo(Variant::INT, "total")));
    ADD_SIGNAL(MethodInfo("event_synchronized_with_peer", PropertyInfo(Variant::INT, "player")));
    ADD_SIGNAL(MethodInfo("event_running"));
    ADD_SIGNAL(MethodInfo("event_disconnected_from_peer", PropertyInfo(Variant::INT, "player")));
    ADD_SIGNAL(MethodInfo("event_timesync", PropertyInfo(Variant::INT, "frames_ahead")));
    ADD_SIGNAL(MethodInfo("event_connection_interrupted", PropertyInfo(Variant::INT, "player"), PropertyInfo(Variant::INT, "disconnect_timeout")));
    ADD_SIGNAL(MethodInfo("event_connection_resumed", PropertyInfo(Variant::INT, "player")));

    BIND_CONSTANT(PLAYERTYPE_LOCAL);
    BIND_CONSTANT(PLAYERTYPE_REMOTE);
    BIND_CONSTANT(PLAYERTYPE_SPECTATOR);
    BIND_CONSTANT(MAX_PLAYERS);
    BIND_CONSTANT(MAX_PREDICTION_FRAMES);
    BIND_CONSTANT(MAX_SPECTATORS);
    BIND_CONSTANT(OK);
    BIND_CONSTANT(INVALID_HANDLE);
    BIND_CONSTANT(ERRORCODE_SUCCESS);
    BIND_CONSTANT(ERRORCODE_GENERAL_FAILURE);
    BIND_CONSTANT(ERRORCODE_INVALID_SESSION);
    BIND_CONSTANT(ERRORCODE_INVALID_PLAYER_HANDLE);
    BIND_CONSTANT(ERRORCODE_PLAYER_OUT_OF_RANGE);
    BIND_CONSTANT(ERRORCODE_PREDICTION_THRESHOLD);
    BIND_CONSTANT(ERRORCODE_UNSUPPORTED);
    BIND_CONSTANT(ERRORCODE_NOT_SYNCHRONIZED);
    BIND_CONSTANT(ERRORCODE_IN_ROLLBACK);
    BIND_CONSTANT(ERRORCODE_INPUT_DROPPED);
    BIND_CONSTANT(ERRORCODE_PLAYER_DISCONNECTED);
    BIND_CONSTANT(ERRORCODE_TOO_MANY_SPECTATORS);
    BIND_CONSTANT(ERRORCODE_INVALID_REQUEST);
    BIND_CONSTANT(EVENTCODE_CONNECTED_TO_PEER);
    BIND_CONSTANT(EVENTCODE_SYNCHRONIZING_WITH_PEER);
    BIND_CONSTANT(EVENTCODE_SYNCHRONIZED_WITH_PEER);
    BIND_CONSTANT(EVENTCODE_RUNNING);
    BIND_CONSTANT(EVENTCODE_DISCONNECTED_FROM_PEER);
    BIND_CONSTANT(EVENTCODE_TIMESYNC);
    BIND_CONSTANT(EVENTCODE_CONNECTION_INTERRUPTED);
    BIND_CONSTANT(EVENTCODE_CONNECTION_RESUMED);
}