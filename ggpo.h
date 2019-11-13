#ifndef GODOT_GGPO_H
#define GODOT_GGPO_H

#include "core/reference.h"
#include "sdk/include/ggponet.h"

class GGPO: public Object {
  GDCLASS(GGPO, Object);

public:
  typedef void (*log_func)(const char* text);
  typedef bool (*begin_game)(const char* game);
  typedef bool (*advance_frame_func)(int flags);
  typedef bool (*load_game_state)(unsigned char* buffer, int length);
  typedef bool (*log_game_state)(char* text, unsigned char* buffer, int length);
  typedef bool (*save_game_state)(unsigned char** buffer, int* len, int* checksum, int frame);
  typedef void (*free_buffer)(void* buffer);
  typedef bool (*on_event)(GGPOEvent* info);
  typedef intptr_t GGPOPtr;

  static GGPO* get_singleton();
  //static log_func getLog;
  static log_func logCallback;
  GGPO();
  ~GGPO();

  const char* plugin_version();
  int plugin_build_number();
  void set_log(log_func callback);
  int start_session(GGPOPtr& sessionRef, begin_game beginGame, advance_frame_func advanceFrame, load_game_state loadGameState, log_game_state logGameState, save_game_state saveGameState, free_buffer freeBuffer, on_event onEvent, const char* game, int numPlayers, int localPort);
  int start_spectating(GGPOPtr& sessionRef, begin_game beginGame, advance_frame_func advanceFrame, load_game_state loadGameState, log_game_state logGameState, save_game_state saveGameState, free_buffer freeBuffer, on_event onEvent, const char* game, int numPlayers, int localPort, char* hostIp, int hostPort);
  int set_disconnect_notify_start(GGPOPtr ggpo, int timeout);
  int set_disconnect_timeout(GGPOPtr ggpo, int timeout);
  int synchronize_input(GGPOPtr ggpo, uint64_t* inputs, int length, int& disconnectFlags);
  int add_local_input(GGPOPtr ggpo, int localPlayerHandle, uint64_t input);
  int close_session(GGPOPtr ggpo);
  int idle(GGPOPtr ggpo, int timeout);
  int add_player(GGPOPtr ggpo, int playerType, int playerNum, const char* playerIpAddress, unsigned short playerPort, int& pHandle);
  int disconnect_player(GGPOPtr ggpo, int pHandle);
  int set_frame_delay(GGPOPtr ggpo, int pHandle, int frameDelay);
  int advance_frame(GGPOPtr ggpo);
  void log(GGPOPtr ggpo, const char* text);
  int get_network_stats(GGPOPtr ggpo, int pHandle, int& sendQueueLen, int& recvQueueLen, int& ping, int& kbpsSent, int& localFramesBehind, int& remoteFramesBehind);

protected:
  static void _bind_methods();
  static GGPO* singleton;
  static const char* PLUGIN_VERSION;
  static const int PLUGIN_BUID_NUMBER;
};

#endif
