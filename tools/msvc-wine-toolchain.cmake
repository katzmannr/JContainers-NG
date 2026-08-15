# See below
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

# Don't execute Windows binaries during try_compile.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#set(CMAKE_TRY_COMPILE_CONFIGURATION Release)

# MSVC-Wine installation root
set(MSVC_ROOT "$ENV{HOME}/msvc")

# Compilers
set(CMAKE_C_COMPILER   "${MSVC_ROOT}/bin/x64/cl.exe")

set(CMAKE_CXX_COMPILER "${MSVC_ROOT}/bin/x64/cl.exe")

# Resource compiler
set(CMAKE_RC_COMPILER "${MSVC_ROOT}/bin/x64/rc.exe")

# Librarian
set(CMAKE_AR "${MSVC_ROOT}/bin/x64/lib.exe")

# Linker
set(CMAKE_LINKER "${MSVC_ROOT}/bin/x64/link.exe")

# Manifest Tool
set(CMAKE_MT "${MSVC_ROOT}/bin/x64/mt.exe")

# Root path for Windows SDK / MSVC
set(CMAKE_FIND_ROOT_PATH
    "${MSVC_ROOT}")
    
# Disable old Ranlib
set(CMAKE_RANLIB /usr/bin/true)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Don't try to run compiler tests
# Only enable if compiler works and boost builds
#set(CMAKE_C_COMPILER_WORKS TRUE)
#set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Produce Windows executables
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

# Default MSVC runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY
    "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
