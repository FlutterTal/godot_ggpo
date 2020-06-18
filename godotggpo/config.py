def can_build(env, platform):
    return platform == "windows" # For now, GGPO isn't available on linux or mac

def configure(env):
    env.Append(CPPPATH=["#modules/godotggpo/sdk/include/"])

    if env["platform"] == "windows":
        if env["CC"] == "cl":
            env.Append(LINKFLAGS=["GGPO.lib"])
            env.Append(LIBPATH=["#modules/godotggpo/sdk/bin"])
        else:
            env.Append(LIBS=["ggpo"])
            env.Append(LIBPATH=["#modules/godotggpo/sdk/bin"])