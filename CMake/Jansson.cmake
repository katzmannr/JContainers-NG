set(CMAKE_POLICY_VERSION_MINIMUM 3.25)
set(JANSSON_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# Prevent jansson from detecting GCC-style symbols when using MSVC
# In some build environments these checks do succeed
set(HAVE_SYNC_BUILTINS OFF CACHE BOOL "" FORCE)
set(HAVE_GETTIMEOFDAY OFF CACHE BOOL "" FORCE)
set(HAVE_SCHED_YIELD OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
  jansson
  GIT_REPOSITORY https://github.com/akheron/jansson.git
  GIT_TAG v2.11
)
FetchContent_MakeAvailable(jansson)
unset(CMAKE_POLICY_VERSION_MINIMUM)
