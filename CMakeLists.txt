cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

################################################################################
# Windows 10 is required. Do NOT increase this to Windows 11.
################################################################################
set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(JContainers LANGUAGES C CXX)

################################################################################
# A current compiler is needed, set it via C++ Standard (stricter code handling)
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
# JContainers-NG does not support Skyrim LE. Don't ask
################################################################################
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project only supports 64-bit builds.")
endif()
message("64bit architecture in use")

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
#if(NOT WIN32)
#    message(FATAL_ERROR "Only WIN32 is supported")
#endif()

if(NOT MSVC)
    message(WARNING "Compiler mismatch. Continue at your own risk.")
else()
    add_compile_options(
        "$<$<COMPILE_LANGUAGE:CXX>:/EHsc>"
    )
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
find_package(googletest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

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
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(spdlog)

# -------------------------
# rapidcsv
# -------------------------
FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
    GIT_TAG v8.92   # check exact version used by CommonLib
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(rapidcsv)

# Provide compatibility variable (important!)
set(RAPIDCSV_INCLUDE_DIRS
    ${rapidcsv_SOURCE_DIR}/src
)

################################################################################
# Boost Package provided by us, but include folders require find_package
################################################################################
find_package(Boost CONFIG REQUIRED)
find_package(rapidcsv CONFIG REQUIRED)

################################################################################
# Sub-projects
################################################################################
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_API_EXAMPLE OFF)
set(COMMONLIBSSE_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(dep/CommonLibSSE-NG)
add_subdirectory(src/JContainers)
add_subdirectory(src/json_validator)

# Attention: ToDo - Example requires larger code changes for CommonLibNG
if(${BUILD_API_EXAMPLE})
    message("Building API Usage Example. Build may break here!")
    add_subdirectory(src/api_usage_example)
else()
    message("Skipping Build of API Usage example")
endif()

add_subdirectory(src/DebugJC)

###############################################################################
# C++ Linker Dependencies
###############################################################################
target_link_libraries(JContainers
    PRIVATE
        jansson
        LuaJIT
        Boost::filesystem
        Boost::thread
        Boost::serialization
        Boost::date_time
)