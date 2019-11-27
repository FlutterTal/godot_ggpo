#include "ggpo.h"
#include <ggponet.h>

#include <string.h>

#include "core/func_ref.h"

GGPO* GGPO::singleton = NULL;

GGPO::GGPO() {
    singleton = this;
}

GGPO::~GGPO() {
    singleton = NULL;
}

GGPO* GGPO::get_singleton() {
    return GGPO::singleton;
}

int GGPO::start_session(const String& game, int numPlayers, int localPort) {
    GGPOSessionCallbacks cb;

    memset(&cb, 0, sizeof(cb));
    cb.begin_game = &GGPO::get_singleton()->beginGame;
    cb.advance_frame = &GGPO::get_singleton()->advanceFrame;
    cb.load_game_state = &GGPO::get_singleton()->loadGameState;
    cb.log_game_state = &GGPO::get_singleton()->logGameState;
    cb.save_game_state = &GGPO::get_singleton()->saveGameState;
    cb.free_buffer = &GGPO::get_singleton()->freeBuffer;
    cb.on_event = &GGPO::get_singleton()->onEvent;

    GGPOSession* ggpo;
    int result = ggpo_start_session(&ggpo, &cb, game.utf8().get_data(), numPlayers, sizeof(uint64_t), localPort);
    GGPO::set_ggpoptr(ggpo);
    return result;
}
int GGPO::start_spectating(const String& game, int numPlayers, int localPort, const String& hostIp, int hostPort) {
    GGPOSessionCallbacks cb;

    memset(&cb, 0, sizeof(cb));
    cb.begin_game = &GGPO::beginGame;
    cb.advance_frame = &GGPO::advanceFrame;
    cb.load_game_state = &GGPO::loadGameState;
    cb.log_game_state = &GGPO::logGameState;
    cb.save_game_state = &GGPO::saveGameState;
    cb.free_buffer = &GGPO::freeBuffer;
    cb.on_event = &GGPO::onEvent;

    GGPOSession* ggpo;
    int result = ggpo_start_spectating(&ggpo, &cb, game.utf8().get_data(), numPlayers, sizeof(uint64_t), localPort, hostIp.utf8().ptrw(), hostPort);
    return result;
}

int GGPO::set_disconnect_notify_start(int timeout) {
    return ggpo_set_disconnect_notify_start(GGPO::get_ggpoptr(), timeout);
}

int GGPO::set_disconnect_timeout(int timeout) {
    return ggpo_set_disconnect_timeout(GGPO::get_ggpoptr(), timeout);
}

int GGPO::synchronize_input(int inputs, int length, int disconnectFlags) {
    return ggpo_synchronize_input(GGPO::get_ggpoptr(), &inputs, sizeof(uint64_t) * length, &disconnectFlags);
}

int GGPO::add_local_input(int localPlayerHandle, uint64_t input) {
    return ggpo_add_local_input(GGPO::get_ggpoptr(), localPlayerHandle, &input, sizeof(uint64_t));
}

int GGPO::close_session() {
    return ggpo_close_session(GGPO::get_ggpoptr());
}

int GGPO::idle(int timeout) {
    return ggpo_idle(GGPO::get_ggpoptr(), timeout);
}

int GGPO::add_player(int playerType, int playerNum, const String& playerIpAddress, int playerPort, int pHandle) {
    GGPOPlayer player;

    player.size = sizeof(GGPOPlayer);
    player.type = (GGPOPlayerType)playerType;
    player.player_num = playerNum;
    strcpy_s(player.u.remote.ip_address, playerIpAddress.utf8().get_data());
    player.u.remote.port = playerPort;

    return ggpo_add_player(GGPO::get_ggpoptr(), &player, &pHandle);
}

int GGPO::disconnect_player(int pHandle) {
    return ggpo_disconnect_player(GGPO::get_ggpoptr(), pHandle);
}

int GGPO::set_frame_delay(int pHandle, int frameDelay) {
    return ggpo_set_frame_delay(GGPO::get_ggpoptr(), pHandle, frameDelay);
}

int GGPO::advance_frame() {
    return ggpo_advance_frame(GGPO::get_ggpoptr());
}

void GGPO::log(const String& text) {
    CharString utf = text.utf8();
    ggpo_log(GGPO::get_ggpoptr(), utf.ptr());
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

bool GGPO::beginGame(const char* game) {
    return FuncRef::call_func(beginGame);
}

bool GGPO::advanceFrame(int flags) {
    return FuncRef::call_func(advanceFrame);
}

bool GGPO::loadGameState(unsigned char* buffer, int length) {
    return FuncRef::call_func(loadGameState);
}

bool GGPO::logGameState(char* filename, unsigned char* buffer, int length) {
    return FuncRef::call_func(logGameState);
}

bool GGPO::saveGameState(unsigned char** buffer, int* length, int* checksum, int frame) {
    return FuncRef::call_func(saveGameState);
}

void GGPO::freeBuffer(void* buffer) {
    FuncRef::set_instance(freeBuffer);
}

bool GGPO::onEvent(GGPOEvent* info) {
    Dictionary event;
    event["code"] = (int)info->code;
    event["connected"] = (int)info->u.connected.player;
    event["synchronizing_player"] = (int)info->u.synchronizing.player;
    event["synchronizing_count"] = (int)info->u.synchronizing.count;
    event["synchronizing_total"] = (int)info->u.synchronizing.total;
    event["synchronized"] = (int)info->u.synchronized.player;
    event["timesync"] = (int)info->u.timesync.frames_ahead;
    event["connection_interupted_player"] = (int)info->u.connection_interrupted.player;
    event["connection_interrupted_timeout"] = (int)info->u.connection_interrupted.disconnect_timeout;
    event["connection_resumed"] = (int)info->u.connection_resumed.player;

    return FuncRef::call_func(onEvent);
}

void GGPO::_bind_methods() {
    ClassDB::bind_method("start_session", &GGPO::start_session);
    ClassDB::bind_method("start_spectating", &GGPO::start_spectating);
    ClassDB::bind_method("set_disconnect_notify_start", &GGPO::set_disconnect_notify_start);
    ClassDB::bind_method("set_disconnect_timeout", &GGPO::set_disconnect_timeout);
    ClassDB::bind_method("synchronize_input", &GGPO::synchronize_input);
    ClassDB::bind_method("add_local_input", &GGPO::add_local_input);
    ClassDB::bind_method("close_session", &GGPO::close_session);
    ClassDB::bind_method("idle", &GGPO::idle);
    ClassDB::bind_method("add_player", &GGPO::add_player);
    ClassDB::bind_method("disconnect_player", &GGPO::disconnect_player);
    ClassDB::bind_method("set_frame_delay", &GGPO::set_frame_delay);
    ClassDB::bind_method("advance_frame", &GGPO::advance_frame);
    ClassDB::bind_method("log", &GGPO::log);
    ClassDB::bind_method("get_network_stats", &GGPO::get_network_stats);
}