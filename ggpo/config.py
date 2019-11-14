def can_build(env, platform):
    return platform == "windows" # For now, GGPO isn't available on linux or mac

def configure(env):
    env.Append(CPPPATH=["#modules/ggpo/sdk/include/"])

    if env["platform"] == "windows":
        if env["CC"] == "cl":
            env.Append(LINKFLAGS=["GGPO.lib"])
            if env["bits"] == "32":
                env.Append(LIBPATH=["#modules/ggpo/sdk/bin/win32"])
            else:
                env.Append(LIBPATH=["#modules/ggpo/sdk/bin/win64"])
        else:
            env.Append(LIBS=["ggpo"])
            if env["bits"] == "32":
                env.Append(LIBPATH=["#modules/ggpo/sdk/bin/win32"])
            else:
                env.Append(LIBPATH=["#modules/ggpo/sdk/bin/win64"])
