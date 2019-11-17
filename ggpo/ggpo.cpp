#include "ggpo.h"
#include <ggponet.h>

#include "core/func_ref.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <cstdio>

GGPO* GGPO::singleton = NULL;

const char* GGPO::PLUGIN_VERSION = "1.0";
const int GGPO::PLUGIN_BUILD_NUMBER = 1;
/*
void callLog(const char* text) {
    if(GGPOSessionWrapper::logCallback)
        GGPOSessionWrapper::log(text);
}

template<typename... Args>
void callLogv(const char* format, Args ... args) {
    size_t size = snprintf(nullptr, 0, format, args ...) + 1;
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format, args ...);
    callLog(buf.get());
}
*/
GGPO::GGPO() {
    singleton = this;
}

GGPO* GGPO::get_singleton() {
    return singleton;
}

GGPO::~GGPO() {
    singleton = NULL;
}

const char* GGPO::plugin_version() {
    return GGPO::PLUGIN_VERSION;
}

const int GGPO::plugin_build_number() {
    return GGPO::PLUGIN_BUILD_NUMBER;
}
/*
void GGPO::set_log(Ref<GGPOSessionWrapper>& callback) {
    logCallback = callback;
}
*/
int GGPO::start_session(Ref<GGPOSessionWrapper>& sessionRef, const String& game, int numPlayers, int localPort) {
    //callLogv("start_session - %s %i %i", game.utf8().get_data(), numPlayers, localPort);
    GGPOSessionCallbacks cb;

    cb.advance_frame = &sessionRef->advance_frame;
	cb.load_game_state = &sessionRef->load_game_state;
	cb.begin_game = &sessionRef->begin_game;
	cb.save_game_state = &sessionRef->save_game_state;
	cb.load_game_state = &sessionRef->load_game_state;
	cb.log_game_state = &sessionRef->log_game_state;
	cb.free_buffer = &sessionRef->free_buffer;
	cb.on_event = &sessionRef->on_event;

    GGPOSession* ggpo;
    int result = ggpo_start_session(&ggpo, &cb, game.utf8().get_data(), numPlayers, sizeof(uint64_t), localPort);
    sessionRef->set_ggpoptr(ggpo);
    return result;
}

int GGPO::start_spectating(Ref<GGPOSessionWrapper>& sessionRef, const String& game, int numPlayers, int localPort, char* hostIp, int hostPort) {
    //callLogv("start_spectating - %s %i %i %s %i", game.utf8().get_data(), numPlayers, localPort, hostIp, hostPort);
    GGPOSessionCallbacks cb;

    cb.advance_frame = &sessionRef->advance_frame;
	cb.load_game_state = &sessionRef->load_game_state;
	cb.begin_game = &sessionRef->begin_game;
	cb.save_game_state = &sessionRef->save_game_state;
	cb.load_game_state = &sessionRef->load_game_state;
	cb.log_game_state = &sessionRef->log_game_state;
	cb.free_buffer = &sessionRef->free_buffer;
	cb.on_event = &sessionRef->on_event;

    GGPOSession* ggpo;
    int result = ggpo_start_spectating(&ggpo, &cb, game.utf8().get_data(), numPlayers, sizeof(uint64_t), localPort, hostIp, hostPort);
    return result;
}

int GGPO::set_disconnect_notify_start(GGPOSession* ggpo, int timeout) {
    //callLog("set_disconnect_notify_start");
    return ggpo_set_disconnect_notify_start(ggpo, timeout);
}

int GGPO::set_disconnect_timeout(GGPOSession* ggpo, int timeout) {
    //callLog("set_disconnect_timeout");
    return ggpo_set_disconnect_timeout(ggpo, timeout);
}

int GGPO::synchronize_input(GGPOSession* ggpo, uint64_t* inputs, int length, int disconnectFlags) {
    //callLog("synchronize_input");
    return ggpo_synchronize_input(ggpo, inputs, sizeof(uint64_t) * length, &disconnectFlags);
}

int GGPO::add_local_input(GGPOSession* ggpo, int localPlayerHandle, uint64_t input) {
    //callLog("add_local_input");
    return ggpo_add_local_input(ggpo, localPlayerHandle, &input, sizeof(uint64_t));
}

int GGPO::close_session(GGPOSession* ggpo) {
    //callLog("close_session");
    return ggpo_close_session(ggpo);
}

int GGPO::idle(GGPOSession* ggpo, int timeout) {
    //callLog("idle");
    return ggpo_idle(ggpo, timeout);
}

int GGPO::add_player(GGPOSession* ggpo, int playerType, int playerNum, const String& playerIpAddress, Variant playerPort, GGPOPlayerHandle pHandle) {
    //callLogv("add_player - %d %d %s %d", playerType, playerNum, playerIpAddress, playerPort);
    GGPOPlayer player;

    player.size = sizeof(GGPOPlayer);
    player.type = (GGPOPlayerType)playerType;
    player.player_num = playerNum;
    player.u.remote.ip_address, playerIpAddress;
    player.u.remote.port = playerPort;

    return ggpo_add_player(ggpo, &player, &pHandle);
}

int GGPO::disconnect_player(GGPOSession* ggpo, int pHandle) {
    //callLog("disconnect_player");
    return ggpo_disconnect_player(ggpo, pHandle);
}

int GGPO::set_frame_delay(GGPOSession* ggpo, int pHandle, int frameDelay) {
    //callLog("set_frame_delay");
    return ggpo_set_frame_delay(ggpo, pHandle, frameDelay);
}

int GGPO::advance_frame(GGPOSession* ggpo) {
    //callLog("advance_frame");
    return ggpo_advance_frame(ggpo);
}

void GGPO::log(GGPOSession* ggpo, const char* text) {
    //callLogv("log - %s", text);
    ggpo_log(ggpo, text);
}

Dictionary GGPO::get_network_stats(Ref<GGPOSessionWrapper>& sessionRef, int pHandle) {
    //callLogv("get_network_stats - %i", pHandle);
    GGPONetworkStats stats;
    Dictionary d;

    d["result"] = ggpo_get_network_stats(sessionRef->get_ggpoptr(), pHandle, &stats);
    d["sendQueueLen"] = stats.network.send_queue_len;
    d["recvQueueLen"] = stats.network.recv_queue_len;
    d["ping"] = stats.network.ping;
    d["kbpsSent"] = stats.network.kbps_sent;
    d["localFramesBehind"] = stats.timesync.local_frames_behind;
    d["remoteFramesBehind"] = stats.timesync.remote_frames_behind;

    return d;
}

void GGPO::_bind_methods() {
    ClassDB::bind_method("plugin_version", &GGPO::plugin_version);
    ClassDB::bind_method("plugin_build_number", &GGPO::plugin_build_number);
    //ClassDB::bind_method("set_log", &GGPO::set_log);
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