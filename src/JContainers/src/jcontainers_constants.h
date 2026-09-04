#pragma once

#include <string_view>
#include <SKSE/SKSE.h>
#include "REL/Relocation.h"
#include <windows.h>
#include <cstdint>

using namespace std::literals;

namespace collections {

#   define JC_API_VERSION           0
#   define JC_FEATURE_VERSION       2
#   define JC_PATCH_VERSION         1
#   define JC_REVISION_VERSION      0

#   define JC_FILE_VERSION          JC_API_VERSION, JC_FEATURE_VERSION, JC_PATCH_VERSION, JC_REVISION_VERSION

#   define JC_VERSION_STR           STR(JC_API_VERSION)           \
                                    "." STR(JC_FEATURE_VERSION)   \
                                    "." STR(JC_PATCH_VERSION)     \
                                    "." STR(JC_REVISION_VERSION)

#   define JC_DATA_FILES            "JCData/"
#   define JC_PLUGIN_NAME           "JContainers"sv

std::string_view plugin_name_init();

class jc_init {

public:
    jc_init() {
        plugin_name_init();
    }
};

static jc_init jcInit;

// Previous constant have been replaced with inline functions (since we have one dll)
static std::string jc_plugin_name;

// Added function for CommonLibSSE-NG that require SkyrimSE.exe

inline bool host_mode()
{
    // Comment out below if your windows environment has issues
    // return std::getenv("JC_CODEGEN") != nullptr;
    static bool value = []{
        wchar_t exe[MAX_PATH];
        auto res = GetModuleFileNameW(nullptr, exe, MAX_PATH);
        if (res != 0) {
            res = (wcsstr(exe, L"python") != nullptr);
        }
        return res;
    }();
    return value;
}

inline bool is_vr()
{
    if (host_mode())
        return false;

    return REL::Module::IsVR();
}

inline REL::Module::Runtime runtime()
{
    if (host_mode())
        return REL::Module::Runtime::AE;   // or whatever default makes sense

    return REL::Module::GetRuntime();
}

const inline std::string plugin_name() {
    return jc_plugin_name;
}

static inline std::string_view plugin_name_init()
{
    if (is_vr()) {
        jc_plugin_name = std::string("JContainersVR");
    } else {
        if (runtime() == REL::Module::Runtime::AE) {
            auto runtimeversion = (host_mode() ? REL::Version("1.6.0") : REL::Module::get().version());
            // Since API does only differentiate between version, 1170 GOG release is unsupported
            // Which means it can be used, but is treated as a Steam release, not a GOG release.
            if (runtimeversion.patch() == 659 || runtimeversion.patch() == 1179)
                jc_plugin_name = std::string("JContainersGOG");
        }
        else {
            jc_plugin_name = std::string("JContainers64");
        }
    }
    return jc_plugin_name;
}

inline std::string_view skse_logs()
{
    if (is_vr()) {
        return "\\My Games\\Skyrim VR\\SKSE\\";
    }

    if (plugin_name() == "JContainersGOG") {
        return "\\My Games\\Skyrim Special Edition GOG\\SKSE\\";
    }

    return "\\My Games\\Skyrim Special Edition\\SKSE\\";
}

inline std::string_view user_files()
{
    if (is_vr()) {
        return "My Games/Skyrim VR/JCUser/";
    }

    if (plugin_name() == "JContainersGOG") {
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
