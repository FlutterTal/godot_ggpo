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
    void reset_singleton();

    const char* plugin_version();
    const int plugin_build_number();
    //void set_log(Ref<GGPOSessionWrapper>& callback);
    int start_session(Ref<GGPOSessionWrapper>& sessionRef, const String& game, int numPlayers, int localPort);
    int start_spectating(Ref<GGPOSessionWrapper>& sessionRef, const String& game, int numPlayers, int localPort, char* hostIp, int hostPort);
    int set_disconnect_notify_start(Ref<GGPOSessionWrapper>& ggpo, int timeout);
    int set_disconnect_timeout(Ref<GGPOSessionWrapper>& ggpo, int timeout);
    int synchronize_input(Ref<GGPOSessionWrapper>& ggpo, int inputs, int length, int disconnectFlags);
    int add_local_input(Ref<GGPOSessionWrapper>& ggpo, int localPlayerHandle, uint64_t input);
    int close_session(Ref<GGPOSessionWrapper>& ggpo);
    int idle(Ref<GGPOSessionWrapper>& ggpo, int timeout);
    int add_player(Ref<GGPOSessionWrapper>& ggpo, int playerType, int playerNum, const String& playerIpAddress, Variant playerPort, GGPOPlayerHandle pHandle);
    int disconnect_player(Ref<GGPOSessionWrapper>& ggpo, int pHandle);
    int set_frame_delay(Ref<GGPOSessionWrapper>& ggpo, int pHandle, int frameDelay);
    int advance_frame(Ref<GGPOSessionWrapper>& ggpo);
    void log(Ref<GGPOSessionWrapper>& ggpo, const char* text);
    Dictionary get_network_stats(Ref<GGPOSessionWrapper>& sessionRef, int pHandle);

protected:
    static void _bind_methods();
    static GGPO* singleton;
    static const char* PLUGIN_VERSION;
    static const int PLUGIN_BUILD_NUMBER;
};

#endif