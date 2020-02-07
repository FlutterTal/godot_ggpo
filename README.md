# Godot GGPO
A custom module in C++ to implement GGPO to Godot Engine

## Build Instructions
Prerequisites:
- SCons
- Visual Studio 2015 or later, or MinGW
- Python 3.5+ (recommended), or Python 2.7+

1. Download the Godot source
1. Download this repo
1. Place the source of this repo in a folder within the `modules` folder for Godot
1. Download the GGPO source code (https://github.com/pond3r/ggpo) and build it for release
1. Place the GGPO files withing the `sdk` folder within our module
1. At the root of the Godot folder run this command `scons platform=windows tools=yes target=release_debug bits=<32 or 64>`

If the build doesn't work, you can download the SDK from here: https://github.com/FlutterTal/godot_ggpo/releases/tag/GGPO
