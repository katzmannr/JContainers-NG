set(PROJECT_NAME JContainers)

################################################################################
# Source folders: - src - src/domain - src/forms - src/object  - src/reflection - src/skse - src/util
#
# Include folders: - ./ - "Data/SKSE/Plugins/JCData/InternalLuaScripts/ - src/api_3
################################################################################
# source_group("" FILES ${no_group_source_files})
# source_group("collections" FILES ${collections})
# source_group("domain_master" FILES ${domain_master})
# source_group("forms" FILES ${forms})
# source_group("gtest" FILES ${gtest})
# source_group("object_module" FILES ${object_module})
# source_group("object_module\\impl" FILES ${object_module__impl})
# source_group("plugin_interface" FILES ${plugin_interface})
# source_group("reflection" FILES ${reflection})
# source_group("reflection\\detail" FILES ${reflection__detail})
# source_group("skse" FILES ${skse})
# source_group("tes_api_3" FILES ${tes_api_3})
# source_group("util" FILES ${util})

file(GLOB ALL_FILES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api_3/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collections/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/domains/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/forms/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/object/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/reflection/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/reflection/detail/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/skse/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp
)

###############################################################################
# CommonLibSSE-NG extension for cmake
###############################################################################
include(${CMAKE_SOURCE_DIR}/dep/CommonLibSSE-NG/cmake/CommonLibSSE.cmake)

################################################################################
# Target
################################################################################
add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES ${ALL_FILES}
    VERSION 0.1.1.0)
#add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/jcontainers_pch.h>"
)

#use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE plugin_example)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION            "TRUE"
)

################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:MultiThreadedDebug>
    $<$<CONFIG:Release>:MultiThreaded>
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/dep/CommonLibSSE-NG/include"
    "${CMAKE_SOURCE_DIR}/dep/CommonLibSSE-NG/CommonLibSSE/include"
    "${CMAKE_SOURCE_DIR}/src"
    "${Boost_INCLUDE_DIRS}"
    "${rapidcsv_SOURCE_DIR}/src"
    "${boost_SOURCE_DIR}/libs/asio/include"
    "${boost_SOURCE_DIR}/libs/lockfree/include"
    "${boost_SOURCE_DIR}/libs/iostreams/include"
    "${boost_SOURCE_DIR}/libs/serialization/include"
    "${Boost_SOURCE_DIR}/libs/uuid/include"
    "${Boost_SOURCE_DIR}/libs/align/include"
    "${Boost_SOURCE_DIR}/libs/parameter/include"
    "${Boost_SOURCE_DIR}/libs/random/include"
    "${Boost_SOURCE_DIR}/libs/tti/include"
    "${jansson_SOURCE_DIR}/src"
    "${jansson_BINARY_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/."
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/jc_interface.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_SOURCE_DIR}/src/JContainers/src"
        FILES ${JC_HEADERS}
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Release>:"
        "NO_JC_DEBUG"
    ">"
    NOMINMAX
    _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
    _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
    _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
    _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
    GTEST_LANG_CXX11
    GTEST_HAS_TR1_TUPLE=0
    _CRT_SECURE_NO_WARNINGS
    WIN32_LEAN_AND_MEAN
    BOOST_HAS_HASH
    _MBCS
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /GL-
            /Od
            /Ob0
            /sdl
            -D_SCL_SECURE_NO_WARNINGS
        >
        $<$<CONFIG:Release>:
            /O1
            /Oi
            /sdl-
            /Os
            /Zi
        >
        /std:c++20
        /GF
        /W3
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /NODEFAULTLIB:LIBCMT.lib
            /DEBUG
        >
        $<$<CONFIG:Release>:
                    /DEBUG:FULL
        >
        /SUBSYSTEM:CONSOLE
        /OPT:REF
        /OPT:ICF
        /INCREMENTAL:NO
    )
endif()

################################################################################
# Post build events
################################################################################
find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        ${Python3_EXECUTABLE}
        -u
        ${CMAKE_SOURCE_DIR}/tools/install.py
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        jansson
        LuaJIT::LuaJIT
        CommonLibSSE::CommonLibSSE
        GTest::gtest_main
        Boost::filesystem
        Boost::headers
        Boost::serialization
        Boost::date_time
)
