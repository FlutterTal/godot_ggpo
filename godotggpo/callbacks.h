#pragma once

#include "ggponet.h"

namespace Callbacks {
    static bool begin_game(const char* game);
    static bool advance_frame(int flags);
    static bool load_game_state(unsigned char* buffer, int len);
    static bool log_game_state(char* filename, unsigned char* buffer, int len);
    static bool save_game_state(unsigned char** buffer, int* len, int* checksum, int frame);
    static void free_buffer(void* buffer);
    static bool on_event(GGPOEvent* info);
}