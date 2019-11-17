#include "register_types"
#include "core/class_db.h"
#include "core/globals.h"

#include "ggpo.h"
#include "ggposessionwrapper.h"


void register_ggpo_types() {
    ClassDB::register_class<GGPO>();
    ClassDB::register_class<GGPOSessionWrapper>();
    Globals::get_singleton()->add_singleton(Globals::Singleton("GGPO", GGPO::get_singleton()));
}

void unregister_ggpo_types() {
    GGPO::reset_singleton();
}