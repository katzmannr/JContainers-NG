set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.8.0
)

FetchContent_MakeAvailable(googletest)
unset(CMAKE_POLICY_VERSION_MINIMUM)
