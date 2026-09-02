# DXTK Shaders Prebuilt Option

For your local linux build copy the precompiled shaders into your directxtk build folder:

1. unpack tools/dxtk.7z archive
2. cp -p tools/dxtk/shaders/ build/MSVC_Wine_Release/_deps/directxtk-build/Shaders/Compiled/

Done. Ignore the rest except you still have a missing shader problem.

# Prebuilding DXTK Shaders

They were generated using (only do this if you do not have the archive or something changed!):

1. prepare directxtk (run cmake)

2. build (wait until build error)

3. Change into directxtk src folder

$ cd build/MSVC_Wine_Release/_deps/directxtk-src/Src/Shaders/

4. Generate, replace \\home\\user\\msvc with your actual user name and folder of msvc.

$ wine cmd.exe /c "set WindowsSdkVerBinPath=Z:\\home\\user\\msvc\\bin&& set FXCARCH=x64&& set CompileShadersOutput=Z:\\home\\user\\JContainers-NG\\build\\MSVC_Wine_Release\\_deps\\directxtk-build\\Shaders\\Compiled&& ./CompileShaders.cmd"

# Build System Details

CMake/directxtk.cmake has a mechanism so the build is no longer triggered:

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)

set(BUILD_TOOLS OFF CACHE BOOL "Build DirectXTK tools" FORCE)

FetchContent_Declare(
  directxtk
  GIT_REPOSITORY https://github.com/microsoft/DirectXTK.git
  GIT_TAG may2026
  OVERRIDE_FIND_PACKAGE
)

set(USE_PREBUILT_SHADERS ON CACHE BOOL "" FORCE)
set(COMPILED_SHADERS
   "${CMAKE_BINARY_DIR}/_deps/directxtk-build/Shaders/Compiled"
   CACHE PATH "" FORCE
)
FetchContent_MakeAvailable(directxtk)
add_library(Microsoft::DirectXTK ALIAS DirectXTK)
unset(BUILD_TOOLS CACHE)
unset(CMAKE_POLICY_VERSION_MINIMUM)

The "PREBUILT_SHADERS" needs to be set and this switch requires to set their own compile folder (typical Microsoft weirdness)
