#pragma once

#include <ggponet.h>

namespace Callbacks {
    static bool beginGame(const char* game);
    static bool advanceFrame(int flags);
    static bool loadGameState(unsigned char* buffer, int length);
    static bool logGameState(char* filename, unsigned char* buffer, int length);
    static bool saveGameState(unsigned char** buffer, int* len, int* checksum, int frame);
    static void freeBuffer(void* buffer);
    static bool onEvent(GGPOEvent* info);
}