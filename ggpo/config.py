def can_build(env, platform):
    return platform == "windows" # For now, ggpo does not exist on linux or mac

def configure(env):
    env.Append(CPPPATH=["#modules/ggpo/libpath"])

    if env["platform"] == "windows":
        env.Append(LINKFLAGS=["ggpo.lib"])
        env.Append(LIBPATH=["#modules/ggpo/libpath"])
