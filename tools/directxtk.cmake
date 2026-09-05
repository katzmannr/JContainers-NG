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
