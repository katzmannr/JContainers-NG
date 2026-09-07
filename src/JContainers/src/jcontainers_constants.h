#pragma once

#include <string_view>
#include <SKSE/SKSE.h>
#include "REL/Relocation.h"
#include <cstdint>

using namespace std::literals;

namespace collections {

// Plugin version, not to be confused with API
#   define JC_PLUGIN_VERSION          0
#   define JC_PLUGIN_MINOR_VERSION    2
#   define JC_PLUGIN_PATCH_VERSION    1
#   define JC_PLUGIN_REVISION_VERSION 0

#   define JC_FILE_VERSION          JC_PLUGIN_VERSION, JC_PLUGIN_MINOR_VERSION, JC_PLUGIN_PATCH_VERSION, JC_PLUGIN_REVISION_VERSION

#   define JC_VERSION_STR           STR(JC_PLUGIN_VERSION)           \
    "." STR(JC_PLUGIN_MINOR_VERSION)   \
    "." STR(JC_PLUGIN_PATCH_VERSION)     \
    "." STR(JC_PLUGIN_REVISION_VERSION)

// API version (same as JC SE, but simplified)
#   define JC_API_MAJOR_VERSION  4
#   define JC_API_MINOR_VERSION  3

#   define JC_API_MIN_MAJOR_VERSION  4
#   define JC_API_MIN_MINOR_VERSION  1

// Use original 4 parts for compatibility
#   define JC_API_VERSION          JC_API_MAJOR_VERSION, JC_API_MINOR_VERSION, JC_PLUGIN_PATCH_VERSION, JC_PLUGIN_REVISION_VERSION

// Use original string for compatibility
#   define JC_API_VERSION_STR           STR(JC_API_MAJOR_VERSION)           \
    "." STR(JC_API_MINOR_VERSION)   \
    "." STR(JC_PLUGIN_PATCH_VERSION)     \
    "." STR(JC_PLUGIN_REVISION_VERSION)


#   define JC_DATA_FILES            "JCData/"
// Attention: Must be converted into string_view for initialization
#   define JC_PLUGIN_NAME           "JContainers"

class jc_constants {
public:
    jc_constants();
private:
    std::string_view plugin_name_init();
};

bool is_vr();
REL::Module::Runtime runtime();
const std::string_view plugin_name();
std::uint32_t engine_version();
std::string_view skse_logs();
std::string_view user_files();

static constexpr std::string_view JC_PLUGIN_FILENAME = JC_PLUGIN_NAME ".dll";

    enum class consts : std::uint32_t {
        storage_chunk = 'JSTR',

        api_version = JC_API_MAJOR_VERSION,
        feature_version = JC_API_MINOR_VERSION,
        api_minimum_version = JC_API_MIN_MAJOR_VERSION,
        feature_minimum_version = JC_API_MIN_MINOR_VERSION,
        patch_version = JC_PLUGIN_PATCH_VERSION,
        revision_version = JC_PLUGIN_REVISION_VERSION,
    };
}
