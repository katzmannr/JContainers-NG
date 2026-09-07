# See below
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

set(MSVC_LIB_RSP_WRAPPER
    "${CMAKE_SOURCE_DIR}/CMake/msvc-lib-rsp.sh")

# Avoid overlong commandlines freezing wine
set(CMAKE_AR "${MSVC_LIB_RSP_WRAPPER}" CACHE FILEPATH "MSVC lib.exe RSP wrapper" FORCE)

# Don't execute Windows binaries during try_compile.
set(CMAKE_TRY_COMPILE_CONFIGURATION Release)

# MSVC-Wine installation root
set(MSVC_ROOT "$ENV{HOME}/msvc")

set(CMAKE_C_ARCHIVE_CREATE
    "<CMAKE_AR> /nologo /machine:x64 /out:<TARGET> <OBJECTS>"
    CACHE STRING "" FORCE)

set(CMAKE_CXX_ARCHIVE_CREATE
    "<CMAKE_AR> /nologo /machine:x64 /out:<TARGET> <OBJECTS>"
    CACHE STRING "" FORCE)

set(CMAKE_C_ARCHIVE_APPEND
    "<CMAKE_AR> /nologo <TARGET> <OBJECTS>"
    CACHE STRING "" FORCE)

set(CMAKE_CXX_ARCHIVE_APPEND
    "<CMAKE_AR> /nologo <TARGET> <OBJECTS>"
    CACHE STRING "" FORCE)

# No special finish step needed
set(CMAKE_C_ARCHIVE_FINISH "" CACHE STRING "" FORCE)
set(CMAKE_CXX_ARCHIVE_FINISH "" CACHE STRING "" FORCE)

# Compilers
set(CMAKE_C_COMPILER   "${MSVC_ROOT}/bin/x64/cl.exe")
set(CMAKE_CXX_COMPILER "${MSVC_ROOT}/bin/x64/cl.exe")
# Resource compiler
set(CMAKE_RC_COMPILER "${MSVC_ROOT}/bin/x64/rc.exe")

# Librarian
#set(CMAKE_AR "${MSVC_ROOT}/bin/x64/lib.exe")

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

# Produce Windows executables
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

# Default MSVC runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY
    "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# message(STATUS "===== TOOLCHAIN TYPE STATE =====")
# message(STATUS "CMAKE_SIZEOF_INT=${CMAKE_SIZEOF_INT}")
# message(STATUS "CMAKE_SIZEOF_LONG=${CMAKE_SIZEOF_LONG}")
# message(STATUS "CMAKE_SIZEOF_LONG_LONG=${CMAKE_SIZEOF_LONG_LONG}")

# message(STATUS "INT=${INT}")
# message(STATUS "HAVE_INT=${HAVE_INT}")

# message(STATUS "LONG_INT=${LONG_INT}")
# message(STATUS "HAVE_LONG_INT=${HAVE_LONG_INT}")

# message(STATUS "LONG_LONG_INT=${LONG_LONG_INT}")
# message(STATUS "HAVE_LONG_LONG_INT=${HAVE_LONG_LONG_INT}")

# message(STATUS "UNSIGNED_INT=${UNSIGNED_INT}")
# message(STATUS "HAVE_UNSIGNED_INT=${HAVE_UNSIGNED_INT}")

# message(STATUS "UNSIGNED_LONG_INT=${UNSIGNED_LONG_INT}")
# message(STATUS "HAVE_UNSIGNED_LONG_INT=${HAVE_UNSIGNED_LONG_INT}")

# message(STATUS "UNSIGNED_SHORT=${UNSIGNED_SHORT}")
# message(STATUS "HAVE_UNSIGNED_SHORT=${HAVE_UNSIGNED_SHORT}")

# message(STATUS "UINT32_T=${UINT32_T}")
# message(STATUS "HAVE_UINT32_T=${HAVE_UINT32_T}")
# message(STATUS "================================")
