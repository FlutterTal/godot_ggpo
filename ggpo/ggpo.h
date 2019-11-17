#ifndef GODOT_GGPO
#define GODOT_GGPO

#include "core/reference.h"
#include "core/object.h"
#include "ggposessionwrapper.h"
#include <ggponet.h>

class GGPO: public Object {
    GDCLASS(GGPO, Object);

public:
    static GGPO* get_singleton();
    GGPO();
    ~GGPO();

    const char* plugin_version();
    const int plugin_build_number();
    //void set_log(Ref<GGPOSessionWrapper>& callback);
    int start_session(Ref<GGPOSessionWrapper>& sessionRef, const String& game, int numPlayers, int localPort);
    int start_spectating(Ref<GGPOSessionWrapper>& sessionRef, const String& game, int numPlayers, int localPort, char* hostIp, int hostPort);
    int set_disconnect_notify_start(GGPOSession* ggpo, int timeout);
    int set_disconnect_timeout(GGPOSession* ggpo, int timeout);
    int synchronize_input(GGPOSession* ggpo, uint64_t* inputs, int length, int disconnectFlags);
    int add_local_input(GGPOSession* ggpo, int localPlayerHandle, uint64_t input);
    int close_session(GGPOSession* ggpo);
    int idle(GGPOSession* ggpo, int timeout);
    int add_player(GGPOSession* ggpo, int playerType, int playerNum, const String& playerIpAddress, Variant playerPort, GGPOPlayerHandle pHandle);
    int disconnect_player(GGPOSession* ggpo, int pHandle);
    int set_frame_delay(GGPOSession* ggpo, int pHandle, int frameDelay);
    int advance_frame(GGPOSession* ggpo);
    void log(GGPOSession* ggpo, const char* text);
    Dictionary get_network_stats(Ref<GGPOSessionWrapper>& sessionRef, int pHandle);

protected:
    static void _bind_methods();
    static GGPO* singleton;
    static const char* PLUGIN_VERSION;
    static const int PLUGIN_BUILD_NUMBER;
};

#endif