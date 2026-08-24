Developer Documentation
=======================

Patches of CommonLibSSE-NG
Made for CharmedByrion 3.7.0 Release
Can be removed if alandtse is properly integrated

Internal: Relocation.h contains additional spdlog output
Can be removed if alandtse is properly integrated

CMakeLists.txt: Locally has changed applied
to build with msvc-wine.

QTCreator IDE:
Use qtcreator-msvc.h to start the Wine MSVC environment
Building is possible inside QTCreator
Used only official release (20.0.1) from Qt.io

CMake sometimes can break. The reason for that is CMake
itself doing too much changes, especially later CMake 4 release.
Best option: Use CMake 3.x for this project
CMake 4.0 early release (up to 4.0.3) are working.

Flatpak: Build / CMake broken with current updates, reason
not know, probably CMake updates. Flatpak likes to do
changes to its container very often breaking scripts.
In general not noticable but for this project very bad.
Do not use QtCreator Flatpak variant (not recommended)


spdlog: skse_callbacks has commented code to activate 
"secret" logging techniques, basically takeover of the
standard logging output which is not a sink and who
knows where it lands for Skyrim (it doesn't have such
a nice Console output as other programs started from cmd)


api_usage_example: Broken, currently not known how
to fix this Source. It does test the API, but that
seems to be a minor part of the code. Too much
assumptions of internals for an example.
Needs major refactoring.


clangd: Difficult because many parts of dependencies
do header size checks that are incompatible with
the clang size. This is independent from the
used IDE. Currently clangd breaks completely, reasons
are unknown.
Known issue: tes_binding.h crashes clangd


Setup for new developers:
========================

a) Linux
1. Download msvc-wine
2. Install it normally
3. Install the older 2022 compiler on top
4. Test it with a simple hello.c and hello.cc
5. Check with wine that both programs run
6. Option: use wineserver / wine boot to speed up building
7. Important: Do not run MO2 in parallel
8. Use the below integration script:

!/usr/bin/env bash
export PATH="$HOME/msvc/x64/bin:/opt/wine-stable/bin:$PATH"
# Will be set by the start script. If really needed change wine-msvc.sh:
# WINE=${WINE:-$(command -v wine64 || command -v wine || false)}
# export WINE=/opt/wine-stable/bin/wine

# Use below export for a different prefix than $HOME/.wine
# export WINEPREFIX="$HOME/.wine-msvc-prefix"

# See above for --env=WINE="$WINE" 
# Use --env=WINEPREFIX="$WINEPREFIX" when setting above export
source "$HOME/msvc/bin/x64/msvcenv.sh" && exec ~/Qt/Tools/QtCreator/bin/qtcreator.sh

This script assumes that msvc is installed in $HOME or ~/ folder.
This script does n ot work with flatpak. If you really want to experiment with that:

#!/usr/bin/env bash
export PATH="$HOME/msvc/x64/bin:/opt/wine-stable/bin:$PATH"
# Will be set by the start script. If really needed change wine-msvc.sh:
# WINE=${WINE:-$(command -v wine64 || command -v wine || false)}
# export WINE=/opt/wine-stable/bin/wine

# Use below export for a different prefix than $HOME/.wine
# export WINEPREFIX="$HOME/.wine-msvc-prefix"

# See above for --env=WINE="$WINE" 
# Use --env=WINEPREFIX="$WINEPREFIX" when setting above export
exec flatpak run --env=PATH="$PATH" --command=bash io.qt.QtCreator -lc ' source "$HOME/msvc/bin/x64/msvcenv.sh" && exec /app/bin/qtcreator '

9. Start qtcreator with that script
10. setup msvc in qtcreator as new compiler. Use "cl" as compiler
11. Use ninja for build (needs to be installed)
12. Note that flatpak likes to install ninja in /sbin which can mess up project qtcreator user files
13. Set Release type (toolchain is made for release)
14. Copy msvc toolchain from tools in "CMake" folder
15. Copy CMakeLists.txt from tools in main folder (diff it before, there may be changes I forgot to add)
16. Clean build folder
17. Download luajit build from github (check one of the builds)
18. Disable luajit build and use the lib instead (see tools: luajit.cmake)
19. Run CMake in QtCreator (Release Mode!)
20. Try to uncomment lower comments if CMake fails early:
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
21. Build it, it will take a while.
22. Ignore the python error or fix it.

You will need a wine python environment to fix that issue.
Also you will need to fix that the correct python from your environment is used.
CMake could still be confused and tries to assume or run Linux environment.
b) Windows

1. Download and install your favorite IDE like QTCreator
2. Install msvc compiler (build tools or full IDE)
3. Setup msvc compiler in IDE or check if it is present
4. Set build type to Release. Everything for debugging is defined in CMakeLists.txt
5. Load CMake project of JContainers
6. Check for errors
7. Build
8. If you want a full build, you need to install python and add path to it in your IDE.

I cannot recommend to debug JContainers. Instead use log output. JContainers is a heavy
multithreaded dll can work at high speed. Add temporary logs or verbose message logs
to temporarily log code parts with high traffic. If API log is disabled, enable it
in tes_api_3.cpp. skse_callbacks registration function is a good start to see if the
functions are properly registered in Papyrus Virtual Machine Registry. WHen this part
works you will be able to see API log.

Code Information:
=================

This section can be improved, but I kept it minimal to parts that most likely will be changed

a) skse_callbacks.cpp: The functions and the class (plugin loading) are like old main()
b) api_3: Actual API containing all those callback functions
c) tes_binding: heart of integration with SKSE / Papyrus Engine ("Template hell")
d) jc_skse: skse API, "real" is the only relevant and used one
e) "string.h": Wrapper for older string type, basically string_view and char*
f) Forms: Original TESForm is wrapped into form_ref
g) bind_traits: Has a lot of Form and other type handling (incl. Errors)


Dependencies
============

All indirect dependencies are added from "CMake" files:
fmt, boost, spdlog, lua, jansson, googletest

CommonLibSSE-NG is a submodule from the original repository
"common" are older functions still in use, but should be
removed at one point in the future

JC_Log: Internal Logging System, based on gLog (IDebugLog).
In addition it prints using console_print from CommonLib.
This needs to be replaced with spdlog. "logging.cpp"

spdlog has most features already implemented, but
since JC_Log is used in a lot of places, it is better
to integrate spdlog directly. 

Currently there are 383 and more parts in JContainers
using boost. At one point getting rid of boost should
be done, but it will be quite an effort to do so.

There has been already one filesystem issue with boost
that was partially resolved by using std::filesystem.
(the other part was related to handling of endings)

Github
======

Github Actions in .github folder are used to 
automatically build JContainers as soon as a
pull request is performed. 

If you prefer to build outside IDE, you can
check here how to build directly from command line.

The artifacts created by github actions are
required for linux local build. For a local Windows
build you need to setup compiler yourself.

Finally make sure to read the official documentation of JContainers!
