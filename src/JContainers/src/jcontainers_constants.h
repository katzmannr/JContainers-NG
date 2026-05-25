#pragma once

#include <cstdint>

namespace collections {

#   define JC_API_VERSION           4
#   define JC_FEATURE_VERSION       2
#   define JC_PATCH_VERSION         13
#   define JC_REVISION_VERSION      1

#   define JC_FILE_VERSION          JC_API_VERSION, JC_FEATURE_VERSION, JC_PATCH_VERSION, JC_REVISION_VERSION

#   define JC_VERSION_STR           STR(JC_API_VERSION)           \
                                    "." STR(JC_FEATURE_VERSION)   \
                                    "." STR(JC_PATCH_VERSION)     \
                                    "." STR(JC_REVISION_VERSION)

#   define JC_DATA_FILES            "JCData/"

// Previous constant have been replaced with inline functions (since we have one dll)

inline std::string_view plugin_name()
{
    if (REL::Module::IsVR()) {
        return "JContainersVR";
    }

    if (REL::Module::GetRuntime() == REL::Module::Runtime::GOG) {
        return "JContainersGOG";
    }

    return "JContainers64";
}

inline std::string_view skse_logs()
{
    if (REL::Module::IsVR()) {
        return "\\My Games\\Skyrim VR\\SKSE\\";
    }

    if (REL::Module::GetRuntime() == REL::Module::Runtime::GOG) {
        return "\\My Games\\Skyrim Special Edition GOG\\SKSE\\";
    }

    return "\\My Games\\Skyrim Special Edition\\SKSE\\";
}

inline std::string_view user_files()
{
    if (REL::Module::IsVR()) {
        return "My Games/Skyrim VR/JCUser/";
    }

    if (REL::Module::GetRuntime() == REL::Module::Runtime::GOG) {
        return "My Games/Skyrim Special Edition GOG/JCUser/";
    }

    return "My Games/Skyrim Special Edition/JCUser/";
}

#   define JC_PLUGIN_FILENAME       plugin_name() + ".dll"

    enum class consts : std::uint32_t {
        storage_chunk = 'JSTR',

        api_version = JC_API_VERSION,
        feature_version = JC_FEATURE_VERSION,
        patch_version = JC_PATCH_VERSION,
        revision_version = JC_REVISION_VERSION,
    };

}
