#ifndef GODOT_GGPOSESSIONWRAPPER
#define GODOT_GGPOSESSIONWRAPPER

#include "core/reference.h"
#include "core/object.h"
#include <ggponet.h>

class GGPOSessionWrapper: public Object {
    GDCLASS(GGPOSessionWrapper, Object);

private:
    GGPOSession* GGPOPtr = NULL;

public:
/*
    void log(const char* text) {
        emit_signal("log", String(text));
    }
*/
    bool begin_game(const char* game) {
        emit_signal("begin_game", String(game));
    }

    bool advance_frame(int flags) {
        emit_signal("advance_frame", flags);
    }

    bool load_game_state(String buffer, int length) {
        emit_signal("load_game_state", String(buffer), length);
    }

    bool log_game_state(char* text, String buffer, int length) {
        emit_signal("log_game_state", String(text), String(buffer), length);
    }

    bool save_game_state(PoolStringArray buffer, int* len, int* checksum, int frame) {
        emit_signal("save_game_state", PoolStringArray(buffer), len, checksum, frame);
    }

    void free_buffer(void* buffer) {
        emit_signal("free_buffer", buffer);
    }

    bool on_event(GGPOEvent* info) {
        emit_signal("on_event", info);
    }

    void _bind_methods() {
        //ADD_SIGNAL(MethodInfo("log", PropertyInfo(Variant::STRING, "text")));
        ADD_SIGNAL(MethodInfo("begin_game", PropertyInfo(Variant::STRING, "game")));
        ADD_SIGNAL(MethodInfo("advance_frame", PropertyInfo(Variant::INT, "flags")));
        ADD_SIGNAL(MethodInfo("load_game_state", PropertyInfo(Variant::STRING, "buffer"), PropertyInfo(Variant::INT, "length")));
        ADD_SIGNAL(MethodInfo("log_game_state", PropertyInfo(Variant::STRING, "text"), PropertyInfo(Variant::STRING, "buffer"), PropertyInfo(Variant::INT, "length")));
        ADD_SIGNAL(MethodInfo("save_game_state", PropertyInfo(Variant::POOL_STRING_ARRAY, "buffer"), PropertyInfo(Variant::INT, "len"), PropertyInfo(Variant::INT, "checksum"), PropertyInfo(Variant::INT, "frame")));
        ADD_SIGNAL(MethodInfo("free_buffer"));
        ADD_SIGNAL(MethodInfo("on_event"));
    }

    GGPOSession* get_ggpoptr() {
        return GGPOPtr;
    }

    void set_ggpoptr(GGPOSession* ggpo) {
        GGPOPtr = ggpo;
    }

    //static FuncRef logCallback;
};

#endif