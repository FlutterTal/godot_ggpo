#include "register_types.h"
#include "core/class_db.h"
#include "core/engine.h"
#include "ggpo.h"

static GGPO* GGPOPtr = NULL;

void register_ggpo_types() {
    ClassDB::register_class<GGPO>();
    GGPOPtr = memnew(GGPO);
    Engine::get_singleton()->add_singleton(Engine::Singleton("GGPO", GGPO::get_singleton()));
}

void unregister_ggpo_types() {
    memdelete<GGPO>(GGPOPtr);
}