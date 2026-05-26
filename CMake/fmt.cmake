set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.2.1
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(fmt)
unset(CMAKE_POLICY_VERSION_MINIMUM)
