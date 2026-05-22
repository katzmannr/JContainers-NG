include(FetchContent)

FetchContent_Declare(
    luajit
    GIT_REPOSITORY https://github.com/LuaJIT/LuaJIT.git
    GIT_TAG v2.1
)

FetchContent_MakeAvailable(LuaJIT)
