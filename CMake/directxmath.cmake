set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)

FetchContent_Declare(
  directxmath
  GIT_REPOSITORY https://github.com/microsoft/DirectXMath.git
  GIT_TAG jun2026
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(directxmath)
unset(CMAKE_POLICY_VERSION_MINIMUM)
