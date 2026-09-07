#pragma once

#include <filesystem>
#include <cstring>
#include <gtest/gtest.h>

#include "jcontainers_constants.h"
#include "master.h"
#include <filesystem>
#include "tes_string.h"
#include "tes_lua.h"
#include "collections/collections.h"
#include "collections/bind_traits.h"

#include <shlobj.h>

namespace tes_api_3 {

/// Redefine in each logging module
#undef  JC_LOG_API_SOURCE
#define JC_LOG_API_SOURCE "JContainers"

    using namespace collections;

    class tes_jcontainers : public class_meta<tes_jcontainers> {
    public:

        REGISTER_TES_NAME("JContainers");

        void additionalSetup() {
            metaInfo.comment = "Utility functionality";
        }

        static bool __isInstalled() {
            return true;
        }
        REGISTERF2_STATELESS(__isInstalled, nullptr, "It's NOT part of public API");

        static UInt32 APIVersion() {
            JC_LOG_API ("version %d",consts::api_version);
            return (UInt32)consts::api_version;
        }
        REGISTERF2_STATELESS(APIVersion, nullptr, []() {
            std::stringstream comm;
            comm << "Version information.\n"
                "It's a good practice to validate installed JContainers version with the following code:\n"
                "    bool isJCValid = JContainers.APIVersion() == AV && JContainers.featureVersion() >= FV\n"
                "where AV and FV are hardcoded API and feature version numbers.\n";
            comm << "Current API version is " << APIVersion() << std::endl;
            comm << "Current feature version is " << featureVersion();
            return comm.str();
        });

        static UInt32 featureVersion() {
            JC_LOG_API ("version %d",consts::feature_version);
            return (UInt32)consts::feature_version;
        }
        REGISTERF2_STATELESS(featureVersion, nullptr, nullptr);

        static UInt32 APIMinVersion() {
            JC_LOG_API ("version %d",consts::api_minimum_version);
            return (UInt32)consts::api_minimum_version;
        }
        REGISTERF2_STATELESS(APIMinVersion, nullptr, []() {
            std::stringstream comm;
            comm << "Version Minimum information.\n"
                    "Use this to validate installed JContainers version when your version requirements are different:\n"
                    "    bool isJCValid = JContainers.APIMinVersion() == minAV && JContainers.featureMinVersion() >= minFV\n"
                    "where AV and FV are hardcoded API and feature version numbers.\n";
            comm << "Current minimum API version is " << APIMinVersion() << std::endl;
            comm << "Current minimum feature version is " << featureMinVersion();
            return comm.str();
        });

        static UInt32 featureMinVersion() {
            JC_LOG_API ("version %d",consts::feature_minimum_version);
            return (UInt32)consts::feature_minimum_version;
        }
        REGISTERF2_STATELESS(featureMinVersion, nullptr, nullptr);

        static UInt32 engineVersion() {
            JC_LOG_API ("version %d",engine_version());
            return (UInt32)engine_version();
        }
        REGISTERF2_STATELESS(engineVersion, nullptr, []() {
            std::stringstream comm;
            comm << "Engine Version information.\n"
                    "Use this to validate use of a specific game engine:\n"
                    "    bool isEngineValid = JContainers.engineVersion() == 1597\n"
                    "where the number is the packed engine version, f.e. 1415 (VR), 1597 (SE), 161179 (GOG).\n"
                    "Could be used with functionality that only exists in certain game engines.\n";
            comm << "Current engine version is " << engineVersion() << std::endl;
            return comm.str();
        });

        static bool fileExistsAtPath(const char *filename)
        {
            JC_LOG_API ("%s", filename ? filename : "");

            if (!filename) {
                return false;
            }

            return std::filesystem::exists(filename);
        }
        REGISTERF2_STATELESS(fileExistsAtPath, "path", "Returns true if the file at a specified @path exists");

        template<class StringList>
        static StringList contentsOfDirectoryAtPath(
            const char *directoryPath
            ,const char *nameEndsWith = "")
        {
            JC_LOG_API ("path %s, %s", directoryPath ? directoryPath : "", nameEndsWith ? nameEndsWith : "");
            JC_LOG_API ("absolute path %s", directoryPath ? std::filesystem::absolute(directoryPath).generic_string().c_str() : "");

            if (!directoryPath) {
                return StringList{};
            }

			std::string file_extension = nameEndsWith ? nameEndsWith : "";
			
			if (!file_extension.empty() && file_extension.front() != '.') {
    			file_extension.insert(file_extension.begin(), '.');
			}

            StringList result{};
            namespace fs = std::filesystem;

            try {
                fs::path root(directoryPath);
                for (fs::directory_iterator itr(root), end_itr; itr != end_itr; ++itr) {
                    const fs::path& path = itr->path();
                    JC_log_full(IDebugLog::kLevel_DebugMessage, "JContainers checking %s", fs::absolute(path).generic_string().c_str());
                    if (!file_extension.empty() ||
                        path.extension().generic_string().compare(file_extension) == 0)
                    {
                        result.emplace_back(itr->path().generic_string());
                    }
                }
            }
            catch (const std::filesystem::filesystem_error& exc) {
                JC_LOG_TES_API_ERROR(JContainsers, contentsOfDirectoryAtPath, "tes_jc throws '%s'", exc.what());
            }

            fs::path root(directoryPath);
            JC_log_full(IDebugLog::kLevel_DebugMessage, "JContainers cwd = %s", fs::current_path().generic_string().c_str());
            JC_log_full(IDebugLog::kLevel_DebugMessage, "JContainers exists = %d directory = %d",
                       fs::exists(root),
                       fs::is_directory(root));
            
			JC_LOG_API("contentsOfDirectoryAtPath -> returning %d files", result.size());
            return result;
        }
        REGISTERF_STATELESS(
            contentsOfDirectoryAtPath<std::vector<skse::string_ref>>, "contentsOfDirectoryAtPath",
            "directoryPath extension=\"\"", nullptr);

        static void removeFileAtPath(const char *filename)
        {
            JC_LOG_API ("%s", filename ? filename : "");

            if (filename) {
                std::filesystem::remove_all(filename);
            }
        }
        REGISTERF2_STATELESS(removeFileAtPath, "path", "Deletes the file or directory identified by the @path");

        static std::string userDirectory()
        {
            JC_LOG_API ("dir %s","custom");

            char path[MAX_PATH];
            if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path))) {
                return std::string();
            }

            strcat_s(path, sizeof(path), (std::string{"/"} + std::string{user_files()}).c_str());

            // race condition possible. hope it's not critical
            if (!std::filesystem::exists(path) && (std::filesystem::create_directories(path), !std::filesystem::exists(path))) {
                return std::string();
            }

            return path;
        }

        static skse::string_ref _userDirectory() {
	        JC_LOG_API ("dir %s",userDirectory().c_str());
            return skse::string_ref(userDirectory().c_str());
        }
        REGISTERF_STATELESS(_userDirectory, "userDirectory", "", std::string{"A path to user-specific directory - "} + std::string{user_files()});

        REGISTER_TEXT([]() {
            const char fmt[] = R"===(
; Returns true if JContainers plugin installed properly
bool function isInstalled() global
    return __isInstalled() && %u == APIVersion() && %u == featureVersion()
endfunction
)===";
            char buff[sizeof(fmt) * 3 / 2] = { '\0' };
            assert(-1 != sprintf_s(buff, fmt, consts::api_version, consts::feature_version));
            return std::string(buff);
        });
    };

    TES_META_INFO(tes_jcontainers);

    TEST(tes_jcontainers, userDirectory)
    {
        JC_LOG_API ("dir %s",tes_jcontainers::userDirectory().c_str());
        tes_context_standalone ctx;

        auto write_file = [&](const std::filesystem::path& path) {
            std::filesystem::remove_all(path);

            EXPECT_FALSE(std::filesystem::is_regular_file(path));

            object_stack_ref obj = tes_object::object<map>(ctx);
            tes_object::writeToFile(ctx, obj.get(), path.string().c_str());

            EXPECT_TRUE(std::filesystem::is_regular_file(path));

            std::filesystem::remove_all(path);
        };

        auto path = tes_jcontainers::userDirectory();
        EXPECT_TRUE(!path.empty());
        EXPECT_TRUE(std::filesystem::is_directory(path));

        write_file(tes_jcontainers::userDirectory() + "/MyMod/123/settings.json");
        write_file(tes_jcontainers::userDirectory() + "/settings.json");
        write_file(tes_jcontainers::userDirectory() + "settings2.json");
        write_file("obj3");
        write_file("path/obj3");
        write_file("/path2/obj3");
        write_file("path3\\obj3");
        write_file("\\path4\\obj3");
    }

    TEST(tes_jcontainers, contentsOfDirectoryAtPath)
    {
	    JC_LOG_API ("%s","arrived at path");
        std::vector<std::string> vec;
        EXPECT_NO_THROW(vec = tes_jcontainers::contentsOfDirectoryAtPath<decltype(vec)>(":invaliddir"));
        EXPECT_TRUE(vec.empty());
    }
}
