cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

################################################################################
# Windows 10 is required. Do NOT increase this to Windows 11.
################################################################################
set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(JContainers LANGUAGES C CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Resolve Dependencies
################################################################################
include(CMake/dependencies.cmake)

cmake_policy(SET CMP0167 OLD)

# Remove comments below if you want to use your own packages (speeds up builds)
#find_package(Boost CONFIG REQUIRED
#    filesystem
#    thread
#    serialization
#    date_time
#)
#find_package(Jansson REQUIRED)
#find_package(LuaJIT REQUIRED)
#find_package(googletest REQUIRED)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


################################################################################
# Disable CMake Import / Export Logic (to stop spdlog EXPORT error)
################################################################################
set(CMAKE_SKIP_INSTALL_RULES ON)

################################################################################
# Resolve CommonLibSSE-NG Dependencies (save you headaches with git versioning)
################################################################################
include(FetchContent)

# -------------------------
# spdlog
# -------------------------
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0   # match CommonLib requirement
)

FetchContent_MakeAvailable(spdlog)

# -------------------------
# rapidcsv
# -------------------------
FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
    GIT_TAG v8.92   # check exact version used by CommonLib
)

FetchContent_MakeAvailable(rapidcsv)

# Provide compatibility variable (important!)
set(RAPIDCSV_INCLUDE_DIRS
    ${rapidcsv_SOURCE_DIR}/src
)

################################################################################
# Sub-projects
################################################################################
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(COMMONLIBSSE_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(dep/CommonLibSSE-NG)
add_subdirectory(src/JContainers)
add_subdirectory(src/json_validator)
add_subdirectory(src/api_usage_example)
add_subdirectory(src/DebugJC)

###############################################################################
# C++ Linker Dependencies
###############################################################################
#target_link_libraries(JContainersTests PRIVATE gtest)
target_link_libraries(JContainers
    PRIVATE
        jansson
        LuaJIT
        Boost::filesystem
        Boost::thread
        Boost::serialization
)