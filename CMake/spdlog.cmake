include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.10.0
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(spdlog)

# spdlog_BINARY_DIR is now populated — force it into the cache
# so CommonLibSSE's find_package(spdlog) resolves here
set(spdlog_DIR ${spdlog_BINARY_DIR} CACHE PATH "")
