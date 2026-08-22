set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(googletest)

if(TARGET GTest::gtest)
    message(STATUS "GTest target exists")
else()
    message(FATAL_ERROR "GTest target missing")
endif()

unset(CMAKE_POLICY_VERSION_MINIMUM)
