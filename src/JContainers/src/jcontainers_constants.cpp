#include "util/to_underlying.hpp"
#include "jcontainers_constants.h"
#include <windows.h>

// Restrict access to variables. These are initialized twice (so no const keyword)
// Internal functions for initialization are no longer exposed in the header
// Implementation details are no longer exposed in the header

namespace collections
{

// Previous constant have been replaced with functions (since we have one dll)
std::string jc_plugin_name = "JContainers";
std::uint32_t jc_engine_version = 0;

bool host_mode();

jc_constants::jc_constants() {
        plugin_name_init();
}

std::string_view jc_constants::plugin_name_init()
{
    jc_plugin_name = "JContainers64";
    if (is_vr()) {
        jc_plugin_name = "JContainersVR";
        jc_engine_version = REL::Module::get().version().pack();
    } else {
        if (runtime() == REL::Module::Runtime::AE) {
            auto runtimeversion = (host_mode() ? REL::Version("1.6.0") : REL::Module::get().version());
            jc_engine_version = runtimeversion.pack();
            // Since API does only differentiate between version, 1170 GOG release is unsupported
            // Which means it can be used, but is treated as a Steam release, not a GOG release.
            if (runtimeversion.patch() == 659 || runtimeversion.patch() == 1179) {
                jc_plugin_name = "JContainersGOG";
            }
            // ToDo: Change name for AE17
            if (runtimeversion.minor() == 7 &&
                (runtimeversion.patch() == 99 || runtimeversion.patch() == 104))
            {
                jc_plugin_name = "JContainers64";
            }
        }
    }
    return jc_plugin_name;
}

// Added function for CommonLibSSE-NG that require SkyrimSE.exe

bool host_mode()
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

bool is_vr()
{
    if (host_mode())
        return false;

    return REL::Module::IsVR();
}

REL::Module::Runtime runtime()
{
    if (host_mode())
        return REL::Module::Runtime::AE;   // or whatever default makes sense

    return REL::Module::GetRuntime();
}

const std::string_view plugin_name() {
    return jc_plugin_name;
}

std::uint32_t engine_version() {
    return jc_engine_version;
}


std::string_view skse_logs()
{
    if (is_vr()) {
        return "\\My Games\\Skyrim VR\\SKSE\\";
    }

    if (plugin_name() == "JContainersGOG") {
        return "\\My Games\\Skyrim Special Edition GOG\\SKSE\\";
    }

    return "\\My Games\\Skyrim Special Edition\\SKSE\\";
}

std::string_view user_files()
{
    if (is_vr()) {
        return "My Games/Skyrim VR/JCUser/";
    }

    if (plugin_name() == "JContainersGOG") {
        return "My Games/Skyrim Special Edition GOG/JCUser/";
    }

    return "My Games/Skyrim Special Edition/JCUser/";
}

}
