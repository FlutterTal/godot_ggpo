#include "register_types.h"
#include "core/class_db.h"
#include "core/engine.h"
#include "godotggpo.h"

static GGPO* GGPOPtr = NULL;

void register_godotggpo_types() {
    ClassDB::register_class<GGPO>();
    GGPOPtr = memnew(GGPO);
    Engine::get_singleton()->add_singleton(Engine::Singleton("GGPO", GGPO::get_singleton()));
}

void unregister_godotggpo_types() {
    memdelete<GGPO>(GGPOPtr);
}