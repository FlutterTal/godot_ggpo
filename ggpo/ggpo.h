#ifndef GODOT_GGPO_H
#define GODOT_GGPO_H

#include "core/reference.h"

#include <ggponet.h>

class GGPO : public Reference {
  GDCLASS(GGPO, Reference);

protected:
  static void _bind_methods();

public:
  typedef void (*log_func)(const char* text);
  typedef bool (*begin_game)(const char* game);
  typedef bool (*save_game_state)(unsigned char** buffer, int* len, int* checksum, int frame);
  typedef bool (*load_game_state)(unsigned char* buffer, int len);
  typedef bool (*log_game_state)(char* filename, unsigned char* buffer, int len);
  typedef void (*free_buffer)(void* buffer);
  typedef bool (*advance_frame)(int flags);
  typedef bool (*on_event)(GGPOEvent* info);
  typedef intptr_t GGPOPtr;

  GGPO();

  const char* pluginVersion();
  int pluginBuildNumber();
  void setLog(log_func callback);
  int startSession(GGPOPtr& sessionRef, begin_game beginGame, advance_frame advanceFrame, load_game_state loadGameState, log_game_state logGameState, save_game_state saveGameState, free_buffer freeBuffer, on_event onEvent, const char* game, int numPlayers, int localport);
  int startSpectating(GGPOPtr& sessionRef, begin_game beginGame, advance_frame advanceFrame, load_game_state loadGameState, log_game_state logGameState, save_game_state saveGameState, free_buffer freeBuffer, on_event onEvent, const char* game, int numPlayers, int localport, char* hostIp, int hostport);
  int setDisconnectNotifyStart(GGPOPtr ggpo, int timeout);
  int setDisconnectTimeout(GGPOPtr ggpo, int timeout);
  int synchronizeInput(GGPOPtr ggpo, uint64_t* inputs, int length, int& disconnectFlags);
  int addLocalInput(GGPOPtr ggpo, int localPlayerHandle, uint64_t input);
  int closeSession(GGPOPtr ggpo);
  int idle(GGPOPtr ggpo, int timeout);
  int addPlayer(GGPOPtr ggpo, int playerType, int playerNum, const char* playerIpAddress, unsigned short playerPort, int& pHandle);
  int disconnectPlayer(GGPOPtr ggpo, int pHandle);
  int setFrameDelay(GGPOPtr ggpo, int pHandle, int frameDelay);
  int advanceFrame(GGPOPtr ggpo);
  void log(GGPOPtr ggpo, const char* text);
  int getNetworkStats(GGPOPtr ggpo, int pHandle, int& sendQueueLen, int& recvQueueLen, int& ping, int& kbpsSent, int& localFramesBehind, int& remoteFramesBehind);
};

#endif
