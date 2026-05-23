set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(JANSSON_BUILD_DOCS OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
  jansson
  GIT_REPOSITORY https://github.com/akheron/jansson.git
  GIT_TAG v2.11
)
FetchContent_MakeAvailable(jansson)
unset(CMAKE_POLICY_VERSION_MINIMUM)
