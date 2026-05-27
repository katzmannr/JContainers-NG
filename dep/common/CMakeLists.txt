set(PROJECT_NAME common64)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "common/IDatabase.inc"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "common/IArchive.h"
    "common/IBufferStream.h"
    "common/IConsole.h"
    "common/ICriticalSection.h"
    "common/IDatabase.h"
    "common/IDataStream.h"
    "common/IDebugLog.h"
    "common/IDirectoryIterator.h"
    "common/IDynamicCreate.h"
    "common/IErrors.h"
    "common/IEvent.h"
    "common/IFIFO.h"
    "common/IFileStream.h"
    "common/IInterlockedLong.h"
    "common/ILinkedList.h"
    "common/IMemPool.h"
    "common/IMutex.h"
    "common/IPipeClient.h"
    "common/IPipeServer.h"
    "common/IPrefix.h"
    "common/IRangeMap.h"
    "common/IReadWriteLock.h"
    "common/ISegmentStream.h"
    "common/ISingleton.h"
    "common/ITextParser.h"
    "common/IThread.h"
    "common/ITimer.h"
    "common/ITypes.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "common/IArchive.cpp"
    "common/IBufferStream.cpp"
    "common/IConsole.cpp"
    "common/IDatabase.cpp"
    "common/IDataStream.cpp"
    "common/IDebugLog.cpp"
    "common/IDirectoryIterator.cpp"
    "common/IDynamicCreate.cpp"
    "common/IErrors.cpp"
    "common/IEvent.cpp"
    "common/IFIFO.cpp"
    "common/IFileStream.cpp"
    "common/IInterlockedLong.cpp"
    "common/IMemPool.cpp"
    "common/IMutex.cpp"
    "common/IPipeClient.cpp"
    "common/IPipeServer.cpp"
    "common/IPrefix.cpp"
    "common/IRangeMap.cpp"
    "common/IReadWriteLock.cpp"
    "common/ISegmentStream.cpp"
    "common/ISingleton.cpp"
    "common/ITextParser.cpp"
    "common/IThread.cpp"
    "common/ITimer.cpp"
    "common/ITypes.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${no_group_source_files}
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE common)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG      "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGGOG   "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGVR    "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_RELEASE    "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEGOG "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEVR  "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/."
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugGOG>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugVR>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugGOG>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugVR>:
                /Od;
                /RTC1
            >
            /W3;
            /Zi;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugGOG>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugVR>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:Release>:
                /Zi
            >
            $<$<CONFIG:ReleaseGOG>:
                /Zi
            >
            $<$<CONFIG:ReleaseVR>:
                /Zi
            >
            /W3;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
endif()



























set(PROJECT_NAME common64)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "common/IDatabase.inc"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "common/IArchive.h"
    "common/IBufferStream.h"
    "common/IConsole.h"
    "common/ICriticalSection.h"
    "common/IDatabase.h"
    "common/IDataStream.h"
    "common/IDebugLog.h"
    "common/IDirectoryIterator.h"
    "common/IDynamicCreate.h"
    "common/IErrors.h"
    "common/IEvent.h"
    "common/IFIFO.h"
    "common/IFileStream.h"
    "common/IInterlockedLong.h"
    "common/ILinkedList.h"
    "common/IMemPool.h"
    "common/IMutex.h"
    "common/IPipeClient.h"
    "common/IPipeServer.h"
    "common/IPrefix.h"
    "common/IRangeMap.h"
    "common/IReadWriteLock.h"
    "common/ISegmentStream.h"
    "common/ISingleton.h"
    "common/ITextParser.h"
    "common/IThread.h"
    "common/ITimer.h"
    "common/ITypes.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "common/IArchive.cpp"
    "common/IBufferStream.cpp"
    "common/IConsole.cpp"
    "common/IDatabase.cpp"
    "common/IDataStream.cpp"
    "common/IDebugLog.cpp"
    "common/IDirectoryIterator.cpp"
    "common/IDynamicCreate.cpp"
    "common/IErrors.cpp"
    "common/IEvent.cpp"
    "common/IFIFO.cpp"
    "common/IFileStream.cpp"
    "common/IInterlockedLong.cpp"
    "common/IMemPool.cpp"
    "common/IMutex.cpp"
    "common/IPipeClient.cpp"
    "common/IPipeServer.cpp"
    "common/IPrefix.cpp"
    "common/IRangeMap.cpp"
    "common/IReadWriteLock.cpp"
    "common/ISegmentStream.cpp"
    "common/ISingleton.cpp"
    "common/ITextParser.cpp"
    "common/IThread.cpp"
    "common/ITimer.cpp"
    "common/ITypes.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${no_group_source_files}
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
#add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE common)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG      "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGGOG   "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGVR    "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_RELEASE    "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEGOG "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEVR  "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/."
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugGOG>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugVR>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugGOG>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugVR>:
                /Od;
                /RTC1
            >
            /W3;
            /Zi;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugGOG>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugVR>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:Release>:
                /Zi
            >
            $<$<CONFIG:ReleaseGOG>:
                /Zi
            >
            $<$<CONFIG:ReleaseVR>:
                /Zi
            >
            /W3;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
endif()



























set(PROJECT_NAME common64)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "common/IDatabase.inc"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "common/IArchive.h"
    "common/IBufferStream.h"
    "common/IConsole.h"
    "common/ICriticalSection.h"
    "common/IDatabase.h"
    "common/IDataStream.h"
    "common/IDebugLog.h"
    "common/IDirectoryIterator.h"
    "common/IDynamicCreate.h"
    "common/IErrors.h"
    "common/IEvent.h"
    "common/IFIFO.h"
    "common/IFileStream.h"
    "common/IInterlockedLong.h"
    "common/ILinkedList.h"
    "common/IMemPool.h"
    "common/IMutex.h"
    "common/IPipeClient.h"
    "common/IPipeServer.h"
    "common/IPrefix.h"
    "common/IRangeMap.h"
    "common/IReadWriteLock.h"
    "common/ISegmentStream.h"
    "common/ISingleton.h"
    "common/ITextParser.h"
    "common/IThread.h"
    "common/ITimer.h"
    "common/ITypes.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "common/IArchive.cpp"
    "common/IBufferStream.cpp"
    "common/IConsole.cpp"
    "common/IDatabase.cpp"
    "common/IDataStream.cpp"
    "common/IDebugLog.cpp"
    "common/IDirectoryIterator.cpp"
    "common/IDynamicCreate.cpp"
    "common/IErrors.cpp"
    "common/IEvent.cpp"
    "common/IFIFO.cpp"
    "common/IFileStream.cpp"
    "common/IInterlockedLong.cpp"
    "common/IMemPool.cpp"
    "common/IMutex.cpp"
    "common/IPipeClient.cpp"
    "common/IPipeServer.cpp"
    "common/IPrefix.cpp"
    "common/IRangeMap.cpp"
    "common/IReadWriteLock.cpp"
    "common/ISegmentStream.cpp"
    "common/ISingleton.cpp"
    "common/ITextParser.cpp"
    "common/IThread.cpp"
    "common/ITimer.cpp"
    "common/ITypes.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${no_group_source_files}
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
#add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE common)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG      "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGGOG   "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGVR    "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_RELEASE    "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEGOG "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEVR  "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/."
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugGOG>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugVR>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugGOG>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugVR>:
                /Od;
                /RTC1
            >
            /W3;
            /Zi;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugGOG>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugVR>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:Release>:
                /Zi
            >
            $<$<CONFIG:ReleaseGOG>:
                /Zi
            >
            $<$<CONFIG:ReleaseVR>:
                /Zi
            >
            /W3;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
endif()



























set(PROJECT_NAME common64)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "common/IDatabase.inc"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "common/IArchive.h"
    "common/IBufferStream.h"
    "common/IConsole.h"
    "common/ICriticalSection.h"
    "common/IDatabase.h"
    "common/IDataStream.h"
    "common/IDebugLog.h"
    "common/IDirectoryIterator.h"
    "common/IDynamicCreate.h"
    "common/IErrors.h"
    "common/IEvent.h"
    "common/IFIFO.h"
    "common/IFileStream.h"
    "common/IInterlockedLong.h"
    "common/ILinkedList.h"
    "common/IMemPool.h"
    "common/IMutex.h"
    "common/IPipeClient.h"
    "common/IPipeServer.h"
    "common/IPrefix.h"
    "common/IRangeMap.h"
    "common/IReadWriteLock.h"
    "common/ISegmentStream.h"
    "common/ISingleton.h"
    "common/ITextParser.h"
    "common/IThread.h"
    "common/ITimer.h"
    "common/ITypes.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "common/IArchive.cpp"
    "common/IBufferStream.cpp"
    "common/IConsole.cpp"
    "common/IDatabase.cpp"
    "common/IDataStream.cpp"
    "common/IDebugLog.cpp"
    "common/IDirectoryIterator.cpp"
    "common/IDynamicCreate.cpp"
    "common/IErrors.cpp"
    "common/IEvent.cpp"
    "common/IFIFO.cpp"
    "common/IFileStream.cpp"
    "common/IInterlockedLong.cpp"
    "common/IMemPool.cpp"
    "common/IMutex.cpp"
    "common/IPipeClient.cpp"
    "common/IPipeServer.cpp"
    "common/IPrefix.cpp"
    "common/IRangeMap.cpp"
    "common/IReadWriteLock.cpp"
    "common/ISegmentStream.cpp"
    "common/ISingleton.cpp"
    "common/ITextParser.cpp"
    "common/IThread.cpp"
    "common/ITimer.cpp"
    "common/ITypes.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${no_group_source_files}
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
#add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE common)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG      "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGGOG   "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGVR    "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_RELEASE    "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEGOG "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEVR  "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/."
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugGOG>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugVR>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugGOG>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugVR>:
                /Od;
                /RTC1
            >
            /W3;
            /Zi;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugGOG>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugVR>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:Release>:
                /Zi
            >
            $<$<CONFIG:ReleaseGOG>:
                /Zi
            >
            $<$<CONFIG:ReleaseVR>:
                /Zi
            >
            /W3;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
endif()



























set(PROJECT_NAME common64)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "common/IDatabase.inc"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "common/IArchive.h"
    "common/IBufferStream.h"
    "common/IConsole.h"
    "common/ICriticalSection.h"
    "common/IDatabase.h"
    "common/IDataStream.h"
    "common/IDebugLog.h"
    "common/IDirectoryIterator.h"
    "common/IDynamicCreate.h"
    "common/IErrors.h"
    "common/IEvent.h"
    "common/IFIFO.h"
    "common/IFileStream.h"
    "common/IInterlockedLong.h"
    "common/ILinkedList.h"
    "common/IMemPool.h"
    "common/IMutex.h"
    "common/IPipeClient.h"
    "common/IPipeServer.h"
    "common/IPrefix.h"
    "common/IRangeMap.h"
    "common/IReadWriteLock.h"
    "common/ISegmentStream.h"
    "common/ISingleton.h"
    "common/ITextParser.h"
    "common/IThread.h"
    "common/ITimer.h"
    "common/ITypes.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "common/IArchive.cpp"
    "common/IBufferStream.cpp"
    "common/IConsole.cpp"
    "common/IDatabase.cpp"
    "common/IDataStream.cpp"
    "common/IDebugLog.cpp"
    "common/IDirectoryIterator.cpp"
    "common/IDynamicCreate.cpp"
    "common/IErrors.cpp"
    "common/IEvent.cpp"
    "common/IFIFO.cpp"
    "common/IFileStream.cpp"
    "common/IInterlockedLong.cpp"
    "common/IMemPool.cpp"
    "common/IMutex.cpp"
    "common/IPipeClient.cpp"
    "common/IPipeServer.cpp"
    "common/IPrefix.cpp"
    "common/IRangeMap.cpp"
    "common/IReadWriteLock.cpp"
    "common/ISegmentStream.cpp"
    "common/ISingleton.cpp"
    "common/ITextParser.cpp"
    "common/IThread.cpp"
    "common/ITimer.cpp"
    "common/ITypes.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${no_group_source_files}
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
#add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE common)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG      "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGGOG   "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGVR    "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_RELEASE    "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEGOG "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEVR  "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/."
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugGOG>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugVR>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugGOG>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugVR>:
                /Od;
                /RTC1
            >
            /W3;
            /Zi;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugGOG>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugVR>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:Release>:
                /Zi
            >
            $<$<CONFIG:ReleaseGOG>:
                /Zi
            >
            $<$<CONFIG:ReleaseVR>:
                /Zi
            >
            /W3;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
endif()



























set(PROJECT_NAME common64)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "common/IDatabase.inc"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "common/IArchive.h"
    "common/IBufferStream.h"
    "common/IConsole.h"
    "common/ICriticalSection.h"
    "common/IDatabase.h"
    "common/IDataStream.h"
    "common/IDebugLog.h"
    "common/IDirectoryIterator.h"
    "common/IDynamicCreate.h"
    "common/IErrors.h"
    "common/IEvent.h"
    "common/IFIFO.h"
    "common/IFileStream.h"
    "common/IInterlockedLong.h"
    "common/ILinkedList.h"
    "common/IMemPool.h"
    "common/IMutex.h"
    "common/IPipeClient.h"
    "common/IPipeServer.h"
    "common/IPrefix.h"
    "common/IRangeMap.h"
    "common/IReadWriteLock.h"
    "common/ISegmentStream.h"
    "common/ISingleton.h"
    "common/ITextParser.h"
    "common/IThread.h"
    "common/ITimer.h"
    "common/ITypes.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "common/IArchive.cpp"
    "common/IBufferStream.cpp"
    "common/IConsole.cpp"
    "common/IDatabase.cpp"
    "common/IDataStream.cpp"
    "common/IDebugLog.cpp"
    "common/IDirectoryIterator.cpp"
    "common/IDynamicCreate.cpp"
    "common/IErrors.cpp"
    "common/IEvent.cpp"
    "common/IFIFO.cpp"
    "common/IFileStream.cpp"
    "common/IInterlockedLong.cpp"
    "common/IMemPool.cpp"
    "common/IMutex.cpp"
    "common/IPipeClient.cpp"
    "common/IPipeServer.cpp"
    "common/IPrefix.cpp"
    "common/IRangeMap.cpp"
    "common/IReadWriteLock.cpp"
    "common/ISegmentStream.cpp"
    "common/ISingleton.cpp"
    "common/ITextParser.cpp"
    "common/IThread.cpp"
    "common/ITimer.cpp"
    "common/ITypes.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${no_group_source_files}
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
#add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE common)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG      "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGGOG   "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_DEBUGVR    "${CMAKE_CURRENT_SOURCE_DIR}/Debug/"
        OUTPUT_DIRECTORY_RELEASE    "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEGOG "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
        OUTPUT_DIRECTORY_RELEASEVR  "${CMAKE_CURRENT_SOURCE_DIR}/Release/"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugGOG>:
            MultiThreadedDebug
        >
        $<$<CONFIG:DebugVR>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseGOG>:
            MultiThreaded
        >
        $<$<CONFIG:ReleaseVR>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugGOG>,$<CONFIG:DebugVR>,$<CONFIG:Release>,$<CONFIG:ReleaseGOG>,$<CONFIG:ReleaseVR>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/."
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugGOG>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:DebugVR>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseGOG>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseVR>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugGOG>:
                /Od;
                /RTC1
            >
            $<$<CONFIG:DebugVR>:
                /Od;
                /RTC1
            >
            /W3;
            /Zi;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugGOG>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:DebugVR>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:Release>:
                /Zi
            >
            $<$<CONFIG:ReleaseGOG>:
                /Zi
            >
            $<$<CONFIG:ReleaseVR>:
                /Zi
            >
            /W3;
            /GR;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
endif()

