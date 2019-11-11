#include "ggpo.h"

#include <stdio.h>
#include "libpath/ggpo/src/include/ggponet.h"

#include <string>
#include <cstdio>

const char* GGPO::PLUGIN_VERSION = "1.0";
const int GGPO::PLUGIN_BUILD_NUMBER = 1;

log_func GGPO::logCallback = NULL;

void callLog(const char* text) {
  if(logCallback)
    logCallback(text);
}

template<typename... Args>
void callLogv(const char* format, Args ... args) {
  size_t size = snprintf(nullptr, 0, format, args ...) + 1;
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format, args ...);
  callLog(buf.get());
}

GGPO::GGPO() {
  logCallback = nullptr;
}

const char* GGPO::pluginVersion() {
  return PLUGIN_VERSION;
}

int GGPO::pluginBuildNumber() {
  return PLUGIN_BUILD_NUMBER;
}

void GGPO::setLog(log_func callback) {
  logCallback = callback;
}

int GGPO::startSession(GGPOPtr& sessionRef, begin_game beginGame, advance_frame advanceFrame, load_game_state loadGameState, log_game_state logGameState, save_game_state saveGameState, free_buffer freeBuffer, on_event onEvent, const char* game, int numPlayers, int localport) {
  callLogv("startSession - %s %i %i", game, numPlayers, localport);
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
  auto result = ggpo_start_session(&ggpo, &cb, game, numPlayers, sizeof(uint64_t), localport);
  sessionRef = (GGPOPtr)ggpo;
  return result;
}

int GGPO::startSpectating(GGPOPtr& sessionRef, begin_game beginGame, advance_frame advanceFrame, load_game_state loadGameState, log_game_state logGameState, save_game_state saveGameState, free_buffer freeBuffer, on_event onEvent, const char* game, int numPlayers, int localport, char* hostIp, int hostport) {
  callLogv("startSpectating - %s %i %i %s %i", game, numPlayers, localport, hostIp, hostport);
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
  auto result = ggpo_start_spectating(&ggpo, &cb, game, numPlayers, sizeof(uint64_t), localport, hostIp, hostport);
  return result;
}

int GGPO::setDisconnectNotifyStart(GGPOPtr ggpo, int timeout) {
  callLog("setDisconnectNotifyStart");
  return ggpo_set_disconnect_notify_start((GGPOSession*)ggpo, timeout);
}

int GGPO::setDisconnectTimeout(GGPOPtr ggpo, int timeout) {
  callLog("setDisconnectTimeout");
  return ggpo_set_disconnect_timeout((GGPOSession*)ggpo, timeout);
}

int GGPO::synchronizeInput(GGPOPtr ggpo, uint64_t* inputs, int length, int& disconnectFlags) {
  callLog("synchronizeInput");
  return ggpo_synchronize_input((GGPOSession*)ggpo, inputs, sizeof(uint64_t) * length, &disconnectFlags);
}

int GGPO::addLocalInput(GGPOPtr ggpo, int localPlayerHandle, uint64_t input) {
  callLog("addLocalInput");
  return ggpo_add_local_input((GGPOSession*)ggpo, localPlayerHandle, &input, sizeof(uint64_t));
}

int GGPO::closeSession(GGPOPtr ggpo) {
  callLog("closeSession");
  return ggpo_close_session((GGPOSession*)ggpo);
}

int GGPO::idle(GGPOPtr ggpo, int timeout) {
  callLog("idle");
  return ggpo_idle((GGPOSession*)ggpo, timeout);
}

int GGPO::addPlayer(GGPOPtr ggpo, int playerType, int playerNum, const char* playerIpAddress, unsigned short playerPort, int& pHandle) {
  callLogv("addPlayer %d %d %s %d", playerType, playerNum, playerIpAddress, playerPort);
  GGPOPlayer player;
  player.size = sizeof(GGPOPlayer);
  player.type = (GGPOPlayerType)playerType;
  player.player_num = playerNum;
  strcpy_s(player.u.remote.ip_address, playerIpAddress);
  player.u.remote.port = playerPort;
  return ggpo_add_player((GGPOSession*)ggpo, &player, &pHandle);
}

int GGPO::disconnectPlayer(GGPOPtr ggpo, int pHandle) {
  callLog("disconnectPlayer");
  return ggpo_disconnect_player((GGPOSession*)ggpo, pHandle);
}

int GGPO::setFrameDelay(GGPOPtr ggpo, int pHandle, int frameDelay) {
  callLog("setFrameDelay");
  return ggpo_set_frame_delay((GGPOSession*)ggpo, pHandle, frameDelay);
}

int GGPO::advanceFrame(GGPOPtr ggpo) {
  callLog("advanceFrame");
  return ggpo_advance_frame((GGPOSession*)ggpo);
}

void GGPO::log(GGPOPtr ggpo, const char* text) {
  callLogv("log %s", text);
  ggpo_log((GGPOSession*)ggpo, text);
}

int GGPO::getNetworkStats(GGPOPtr ggpo, int pHandle, int& sendQueueLen, int& recvQueueLen, int& ping, int& kbpsSent, int& localFramesBehind, int& remoteFramesBehind) {
  callLogv("getNetworkStats %i", pHandle);
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
  ClassDB::bind_method("pluginVersion", &GGPO::pluginVersion);
  ClassDB::bind_method("pluginBuildNumber", &GGPO::pluginBuildNumber);
  ClassDB::bind_method("setLog", &GGPO::setLog);
  ClassDB::bind_method("startSession", &GGPO::startSession);
  ClassDB::bind_method("startSpectating", &GGPO::startSpectating);
  ClassDB::bind_method("setDisconnectNotifyStart", &GGPO::setDisconnectNotifyStart);
  ClassDB::bind_method("setDisconnectTimeout", &GGPO::setDisconnectTimeout);
  ClassDB::bind_method("synchronizeInput", &GGPO::synchronizeInput);
  ClassDB::bind_method("addLocalInput", &GGPO::addLocalInput);
  ClassDB::bind_method("closeSession", &GGPO::closeSession);
  ClassDB::bind_method("idle", &GGPO::idle);
  ClassDB::bind_method("addPlayer", &GGPO::addPlayer);
  ClassDB::bind_method("disconnectPlayer", &GGPO::disconnectPlayer);
  ClassDB::bind_method("setFrameDelay", &GGPO::setFrameDelay);
  ClassDB::bind_method("advanceFrame", &GGPO::advanceFrame);
  ClassDB::bind_method("log", &GGPO::log);
  ClassDB::bind_method("getNetworkStats", &GGPO::getNetworkStats);
}
