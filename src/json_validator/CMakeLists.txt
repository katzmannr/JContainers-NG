set(PROJECT_NAME json_validator)

################################################################################
# Boost Package provided by us, but include folders require find_package
################################################################################
find_package(Boost CONFIG REQUIRED)

################################################################################
# Source groups
################################################################################
file(GLOB ALL_FILES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

#use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE json_validator)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
    INTERPROCEDURAL_OPTIMIZATION            "TRUE"
)

################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:Release>:
        MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
#target_include_directories(${PROJECT_NAME} PRIVATE
#    ${boost_SOURCE_DIR}/libs/filesystem/include
#)
target_include_directories(${PROJECT_NAME} PUBLIC
    "${boost_SOURCE_DIR}/libs/lockfree/include"
    "${boost_SOURCE_DIR}/libs/iostreams/include"
    "${CMAKE_SOURCE_DIR}/src/JContainers/src"
    "${jansson_SOURCE_DIR}/src"
    "${jansson_BINARY_DIR}/include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    UNICODE
    _UNICODE
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /GL-
            /Od
            /Oi-
            /sdl
            /RTC1
        >
        $<$<CONFIG:Release>:
            /O2
            /Oi
            /Gy
        >
        /W3
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
#       /Za # breaks boost build
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF
            /OPT:ICF
        >
        /DEBUG
        /SUBSYSTEM:CONSOLE
        /INCREMENTAL:NO
    )
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        jansson
        Boost::filesystem
        Boost::headers
        rapidcsv
)
