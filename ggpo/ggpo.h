#ifndef GODOTGGPO_H
#define GODOTGGPO_H

#include "core/reference.h"
#include "core/object.h"
#include "core/dictionary.h"
#include <ggponet.h>

class GGPO: public Object {
    GDCLASS(GGPO, Object);

public:
    GGPO();
    ~GGPO();
    static GGPO* get_singleton();

    int start_session(const String& game, int numPlayers, int localPort);
    int start_spectating(const String& game, int numPlayers, int localPort, const String& hostIp, int hostPort);
    int set_disconnect_notify_start(int timeout);
    int set_disconnect_timeout(int timeout);
    int synchronize_input(int inputs, int length, int disconnectFlags);
    int add_local_input(int localPlayerHandle, uint64_t input);
    int close_session();
    int idle(int timeout);
    int add_player(int playerType, int playerNum, const String& playerIpAddress, int playerPort, int pHandle);
    int disconnect_player(int pHandle);
    int set_frame_delay(int pHandle, int frameDelay);
    int advance_frame();
    void log(const String& text);
    Dictionary get_network_stats(int pHandle);

    GGPOSession* get_ggpoptr() {
        return GGPOPtr;
    }
    void set_ggpoptr(GGPOSession* ggpo) {
        GGPOPtr = ggpo;
    }

    bool beginGame(const char* game);
    bool advanceFrame(int flags);
    bool loadGameState(unsigned char* buffer, int length);
    bool logGameState(char* filename, unsigned char* buffer, int length);
    bool saveGameState(unsigned char** buffer, int* length, int* checksum, int frame);
    void freeBuffer(void* buffer);
    bool onEvent(GGPOEvent* info);

protected:
    static void _bind_methods();
    static GGPO* singleton;
    GGPOSession* GGPOPtr = NULL;
};

#endif //GODOTGGPO_H