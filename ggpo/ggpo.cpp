#include "ggpo.h"
#include <ggponet.h>

#include "core/method_bind_ext.gen.inc"

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <cstdio>

GGPO* GGPO::singleton = NULL;

const char* GGPO::PLUGIN_VERSION = "1.0";
const int GGPO::PLUGIN_BUID_NUMBER = 1;

GGPO::log_func logCallback = nullptr;

void callLog(const char* text) {
  if(GGPO::logCallback)
    GGPO::logCallback(text);
}

template<typename... Args>
void callLogv(const char* format, Args ... args) {
  size_t size = snprintf(nullptr, 0, format, args ...) + 1;
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format, args ...);
  callLog(buf.get());
}

GGPO::GGPO() {
  GGPO::logCallback = nullptr;
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

int GGPO::plugin_build_number() {
  return GGPO::PLUGIN_BUID_NUMBER;
}

void GGPO::set_log(log_func callback) {
  GGPO::logCallback = callback;
}

int GGPO::start_session(GGPOPtr& sessionRef, begin_game beginGame, advance_frame_func advanceFrame, load_game_state loadGameState, log_game_state logGameState, save_game_state saveGameState, free_buffer freeBuffer, on_event onEvent, const char* game, int numPlayers, int localPort) {
  callLogv("start_session - %s %i %i", game, numPlayers, localPort);
  GGPOSessionCallbacks cb;
  cb.advance_frame = advanceFrame;
  cb.load_game_state = loadGameState;
  cb.begin_game = beginGame;
  cb.save_game_state = saveGameState;
  cb.load_game_state = loadGameState;
  cb.log_game_state = logGameState;
  cb.free_buffer = freeBuffer;
  cb.on_event = onEvent;

  GGPOSession* ggpo;
  auto result = ggpo_start_session(&ggpo, &cb, game, numPlayers, sizeof(uint64_t), localPort);
  sessionRef = (GGPOPtr)ggpo;
  return result;
}

int GGPO::start_spectating(GGPOPtr& sessionRef, begin_game beginGame, advance_frame_func advanceFrame, load_game_state loadGameState, log_game_state logGameState, save_game_state saveGameState, free_buffer freeBuffer, on_event onEvent, const char* game, int numPlayers, int localPort, char* hostIp, int hostPort) {
  callLogv("start_spectating - %s %i %i %s %i", game, numPlayers, localPort, hostIp, hostPort);
  GGPOSessionCallbacks cb;
  cb.advance_frame = advanceFrame;
  cb.load_game_state = loadGameState;
  cb.begin_game = beginGame;
  cb.save_game_state = saveGameState;
  cb.load_game_state = loadGameState;
  cb.log_game_state = logGameState;
  cb.free_buffer = freeBuffer;
  cb.on_event = onEvent;

  GGPOSession* ggpo;
  auto result = ggpo_start_spectating(&ggpo, &cb, game, numPlayers, sizeof(uint64_t), localPort, hostIp, hostPort);
  return result;
}

int GGPO::set_disconnect_notify_start(GGPOPtr ggpo, int timeout) {
  callLog("set_disconnect_notify_start");
  return ggpo_set_disconnect_notify_start((GGPOSession*)ggpo, timeout);
}

int GGPO::set_disconnect_timeout(GGPOPtr ggpo, int timeout) {
  callLog("set_disconnect_timeout");
  return ggpo_set_disconnect_timeout((GGPOSession*)ggpo, timeout);
}

int GGPO::synchronize_input(GGPOPtr ggpo, uint64_t* inputs, int length, int& disconnectFlags) {
  callLog("synchronize_input");
  return ggpo_synchronize_input((GGPOSession*)ggpo, inputs, sizeof(uint64_t) * length, &disconnectFlags);
}

int GGPO::add_local_input(GGPOPtr ggpo, int localPlayerHandle, uint64_t input) {
  callLog("add_local_input");
  return ggpo_add_local_input((GGPOSession*)ggpo, localPlayerHandle, &input, sizeof(uint64_t));
}

int GGPO::close_session(GGPOPtr ggpo) {
  callLog("close_session");
  return ggpo_close_session((GGPOSession*)ggpo);
}

int GGPO::idle(GGPOPtr ggpo, int timeout) {
  callLog("idle");
  return ggpo_idle((GGPOSession*)ggpo, timeout);
}

int GGPO::add_player(GGPOPtr ggpo, int playerType, int playerNum, const char* playerIpAddress, unsigned short playerPort, int& pHandle) {
  callLogv("add_player - %d %d %s %d", playerType, playerNum, playerIpAddress, playerPort);
  GGPOPlayer player;
  player.size = sizeof(GGPOPlayer);
  player.type = (GGPOPlayerType)playerType;
  player.player_num = playerNum;
  strcpy_s(player.u.remote.ip_address, playerIpAddress);
  player.u.remote.port = playerPort;
  return ggpo_add_player((GGPOSession*)ggpo, &player, &pHandle);
}

int GGPO::disconnect_player(GGPOPtr ggpo, int pHandle) {
  callLog("disconnect_player");
  return ggpo_disconnect_player((GGPOSession*)ggpo, pHandle);
}

int GGPO::set_frame_delay(GGPOPtr ggpo, int pHandle, int frameDelay) {
  callLog("set_frame_delay");
  return ggpo_set_frame_delay((GGPOSession*)ggpo, pHandle, frameDelay);
}

int GGPO::advance_frame(GGPOPtr ggpo) {
  callLog("advance_frame");
  return ggpo_advance_frame((GGPOSession*)ggpo);
}

void GGPO::log(GGPOPtr ggpo, const char* text) {
  callLogv("log - %s", text);
  ggpo_log((GGPOSession*)ggpo, text);
}

int GGPO::get_network_stats(GGPOPtr ggpo, int pHandle, int& sendQueueLen, int& recvQueueLen, int& ping, int& kbpsSent, int& localFramesBehind, int& remoteFramesBehind) {
  callLogv("get_network_stats - %i", pHandle);
  GGPONetworkStats stats;
  auto result = ggpo_get_network_stats((GGPOSession*)ggpo, pHandle, &stats);
  sendQueueLen = stats.network.send_queue_len;
  recvQueueLen = stats.network.recv_queue_len;
  ping = stats.network.ping;
  kbpsSent = stats.network.kbps_sent;
  localFramesBehind = stats.timesync.local_frames_behind;
  remoteFramesBehind = stats.timesync.remote_frames_behind;
  return result;
}

void GGPO::_bind_methods() {
  ClassDB::bind_method("plugin_version", &GGPO::plugin_version);
  ClassDB::bind_method("plugin_build_number", &GGPO::plugin_build_number);
  ClassDB::bind_method("set_log", &GGPO::set_log);
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
