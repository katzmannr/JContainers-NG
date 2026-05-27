set(PROJECT_NAME common)

################################################################################
# Source groups
################################################################################
#set(no_group_source_files
#    "common/IDatabase.inc"
#)
#source_group("" FILES ${no_group_source_files})

file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${COMMON_SOURCES})
add_library(jc::common ALIAS ${PROJECT_NAME})

#use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE common)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG      "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
    OUTPUT_DIRECTORY_RELEASE    "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
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
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

################################################################################
# Compile features + definitions
################################################################################
target_compile_features(common PUBLIC cxx_std_20)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_MBCS"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /RTC1
        >
        /W3;
        /Zi;
        /GR;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
endif()
