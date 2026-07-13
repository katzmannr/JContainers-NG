include(ExternalProject)

set(LUAJIT_PREFIX "${CMAKE_BINARY_DIR}/extern-luajit")
set(LUAJIT_INSTALL_DIR "${LUAJIT_PREFIX}/install")

# Ensure the install dir exists at configure time
file(MAKE_DIRECTORY
    "${LUAJIT_INSTALL_DIR}/include"
)

# External project, this part is run later during build.
# Since luajit creates dependencies, you need to build this
# part first before being able to create cose based on lua.h.
ExternalProject_Add(
    LuaJIT
    PREFIX ${LUAJIT_PREFIX}
    GIT_REPOSITORY https://github.com/LuaJIT/LuaJIT.git
    GIT_TAG v2.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>/src
        cmd /c msvcbuild.bat static
    BUILD_IN_SOURCE TRUE
    INSTALL_DIR     ${LUAJIT_INSTALL_DIR}
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/include &&
        ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/lib &&
        ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/lua.hpp        <INSTALL_DIR>/include/ &&
        ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/lua.h          <INSTALL_DIR>/include/ &&
        ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/lualib.h       <INSTALL_DIR>/include/ &&
        ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/lauxlib.h      <INSTALL_DIR>/include/ &&
        ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/luajit.h       <INSTALL_DIR>/include/ &&
        ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/luaconf.h      <INSTALL_DIR>/include/ &&
        ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/luajit.lib     <INSTALL_DIR>/lib/
        ${CMAKE_COMMAND} -E chdir ${LUAJIT_INSTALL_DIR} cmd /c dir /s
)

ExternalProject_Get_Property(LuaJIT INSTALL_DIR)

add_library(LuaJIT::LuaJIT STATIC IMPORTED GLOBAL)

set_target_properties(LuaJIT::LuaJIT PROPERTIES
    IMPORTED_LOCATION             "${INSTALL_DIR}/lib/luajit.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
)

add_dependencies(LuaJIT::LuaJIT LuaJIT)

message(STATUS "LuaJIT prepare step done.")