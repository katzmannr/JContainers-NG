#include <boost/iostreams/stream.hpp>
#include "util/to_underlying.hpp"
#include "RE/B/BSCoreTypes.h"
#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include "RE/I/IVirtualMachine.h"
#include "SKSE/Interfaces.h"
#include <shlobj.h>


#include <SKSE/SKSE.h>

#include "SkyrimVRESLAPI.h"

#include "common/IDebugLog.h"
#include "common/ITypes.h"
#include "spdlog/common.h"
#include <spdlog/sinks/basic_file_sink.h>
#include "typedefs.h"
#include "util/util.h"
#include "reflection/reflection.h"

#include "collections/context.h"
#include "forms/form_observer.h"

#include "domains/domain_master.h"
#include "jc_interface.h"
#include "jcontainers_constants.h"

namespace jc {
    extern root_interface root;
}

namespace {

using namespace collections;
using namespace domain_master;

class skse_callbacks {

public:
    skse_callbacks() {
        m_serialization = SKSE::GetSerializationInterface();
        m_papyrus = SKSE::GetPapyrusInterface();
        if (!m_papyrus) {
            JC_log_full(IDebugLog::LogLevel::kLevel_Error,"skse callback: couldn't get papyrus interface");
            return;
        }
        s_messaging = SKSE::GetMessagingInterface();
        JC_log_full(IDebugLog::kLevel_DebugMessage,"skse callback: s_messaging 0x%X",s_messaging);
    }

    static void revert(SKSE::SerializationInterface *intfc) {
        JC_log_full(IDebugLog::kLevel_DebugMessage,"skse callback: revert");
        util::do_with_timing("Revert", []() {
            jc_skse::set_silent_api();
            domain_master::master::instance().clear_state();
            jc_skse::set_real_api();
        });
    }

    static void save(SKSE::SerializationInterface * intfc) {
        JC_log_full(IDebugLog::kLevel_DebugMessage,"skse callback: save");

        namespace io = boost::iostreams;

        struct skse_data_sink {
            typedef char      char_type;
            typedef io::sink_tag  category;

            std::streamsize write(const char* s, std::streamsize n) const {
                (void)_sink->WriteRecordData(s, n); // always returns true
                return n;
            }

            SKSE::SerializationInterface* _sink;
        };


        util::do_with_timing("Save", [intfc]() {
                if (intfc->OpenRecord((UInt32)consts::storage_chunk, (UInt32)serialization_version::current)) {
                    io::stream<skse_data_sink> stream(skse_data_sink{ intfc });
                    domain_master::master::instance().write_to_stream(stream);
                    //_DMESSAGE("%lu bytes saved", stream.tellp());
                }
                else {
                    JC_log_full(IDebugLog::LogLevel::kLevel_Error, "skse callback: Unable open JC record");
                }
            });
        }

        static void load(SKSE::SerializationInterface* intfc) {
            JC_log_full(IDebugLog::kLevel_DebugMessage,"skse callback: load");

            namespace io = boost::iostreams;

            class skse_data_source {
            public:
                typedef char char_type;
                typedef io::source_tag  category;

                explicit skse_data_source(SKSE::SerializationInterface* src = nullptr) : _source(src){}

                std::streamsize read(char* buffer, std::streamsize n) const {
                    return _source ? _source->ReadRecordData(buffer, n) : 0;
                }

            private:
                SKSE::SerializationInterface* _source;
            };

            util::do_with_timing("Load", [intfc]() {

                jc_skse::set_silent_api();
                domain_master::master::instance().clear_state();
                jc_skse::set_real_api();

                UInt32 type = 0;
                UInt32 version = 0;
                UInt32 length = 0;

                while (intfc->GetNextRecordInfo(type, version, length)) {
                    JC_log_full(IDebugLog::kLevel_VerboseMessage,"skse callback: GetNextRecordInfo t %d v  %d",type,version);
                    if (static_cast<consts>(type) == consts::storage_chunk) {
                        break;
                    }
                }

                io::stream<skse_data_source> stream(skse_data_source(static_cast<consts>(type) == consts::storage_chunk ? intfc : nullptr));
                domain_master::master::instance().read_from_stream(stream);
            });
        }

        static void delet(RE::VMHandle handle) {
            JC_log_full(IDebugLog::kLevel_VerboseMessage, "skse callback: delet, handle=%016llX",handle);
            domain_master::master::instance().get_form_observer().on_form_deleted((forms::FormHandle)handle);
        }

        static void listene(SKSE::MessagingInterface::Message* msg) {
            if( msg == nullptr ) {
                JC_log_full(IDebugLog::kLevel_Warning,"skse callback: Message: Invalid");
                return;
            }
            JC_log_full(
                IDebugLog::kLevel_DebugMessage,"skse callback: Message from %s type=%u dataLen=%u data=%p",
                    (msg->sender ? msg->sender : "unknown"), msg->type, msg->dataLen, msg->data);
            if (msg->type == SKSE::MessagingInterface::kPostPostLoad) {
                s_messaging->Dispatch(jc::message_root_interface, (void *)&jc::root, sizeof(void*), nullptr);
                if (REL::Module::IsVR())
                {
                    SkyrimVRESLPluginAPI::GetSkyrimVRESLInterface001(s_pluginHandle, s_messaging);
                    if (g_SkyrimVRESLInterface)
                    {
                        JC_log("skse callback: SkyrimVRESL interface detected and initialized!");
                    }
                    else
                    {
                        JC_log("skse callback: SkyrimVRESL interface is not present or has failed to be retrieved... ESL related functionality is disabled.");
                    }
                }
            }
        }

        // Internal Query interface to checka and store object provided by SKSE
        bool SKSEQuery (const SKSE::LoadInterface * skse)
        {
            JC_log_full(IDebugLog::kLevel_DebugMessage, "skse callback: Query");

            // store plugin handle so we can identify ourselves later
            s_pluginHandle = skse->GetPluginHandle();
            JC_log_full(IDebugLog::kLevel_DebugMessage, "skse callback: Query Handle: 0x%X", s_pluginHandle );


            if (skse->IsEditor()) {
                JC_log_full(IDebugLog::kLevel_Error, "skse callback: loaded in editor, marking as incompatible");
                return false;
            }

            // get the serialization interface and query its version
            m_serialization = (SKSE::SerializationInterface *)skse->QueryInterface(SKSE::LoadInterface::kSerialization);
            if (!m_serialization) {
                JC_log_full(IDebugLog::kLevel_Error, "skse callback: couldn't get serialization interface");
                return false;
            }

            if (m_serialization->Version() < SKSE::SerializationInterface::kVersion) {
                JC_log_full(IDebugLog::kLevel_Error, "skse callback: serialization interface too old (%d expected %d)", m_serialization->Version(), SKSE::SerializationInterface::kVersion);
                return false;
            }

            m_papyrus = (SKSE::PapyrusInterface *)skse->QueryInterface(SKSE::LoadInterface::kPapyrus);

            if (!m_papyrus) {
                JC_log_full(IDebugLog::kLevel_Error, "skse callback: couldn't get papyrus interface");
                return false;
            }

            auto messaging = (SKSE::MessagingInterface *)skse->QueryInterface(SKSE::LoadInterface::kMessaging);
            if (messaging && messaging->Version() >= SKSE::MessagingInterface::kVersion) {
                s_messaging = messaging;
            } else {
                JC_log_full(IDebugLog::kLevel_Warning, "skse callback: couldn't get messaging interface: Incorrect Version %d.", messaging->Version());
            }

            return true;
        }

        static bool registerAllFunctions(RE::BSScript::IVirtualMachine *vm) {

            // Registration is done when plugin is loaded. Remove all those logs until here is required to reenable it.
            // gLog.OpenRelative(CSIDL_MYDOCUMENTS, (std::string(skse_logs()) + std::string(plugin_name()) + ".log").c_str());
            // gLog.SetPrintLevel(IDebugLog::kLevel_Error);
            // gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

            // store plugin handle so we can identify ourselves later
            s_pluginHandle = SKSE::GetPluginHandle();

            // This old check could be useful in a rare case of multiple mixed version build
            // messaging && messaging->interfaceVersion >= SKSE::MessagingInterface::kVersion)

            JC_log_full(IDebugLog::kLevel_DebugMessage, "registering All functions of %s Version %s", plugin_name().data(), JC_VERSION_STR);

            jc_assert(vm);
            if (vm==nullptr) {
                JC_log_full(IDebugLog::kLevel_Error, "skse callback: registerAllFunctions: Invalid vm");
                return false;
            }

            // One of the ways: temp. clone class meta infos, register them
            // 2nd: pass each context into "info.bind(*registry, some-context);"

            // Anyway, in a result we must construct plenty of SKSE native functors, each function will store a pointer
            // to tes_context instance. The functions will be registered in Papyrus VM

            // context_master should return tes_context's for us
            // Here we should iterator over files in JCData/Contexts/ folder:
            // for f in "./JCData/Contexts/*.json":
            //   c = master.get_context(f)
            //   for cls in metainfo:
            //     cls.register(c,f)

            // Need to enhance control over resulting function and class name
            // E.g. turn JArray.addObj into PSM_JContainers.JArray_addObj

            // Pitfall: since the functions registered only ONCE, we must
            // preserve context pointers during ALL gaming session

            // ����� ��������, ��� ����� ���� ���� ����� �������� ���� �� ��������

            util::do_with_timing("Registering functions", [=]() {

                auto& master = domain_master::master::instance();

                auto registerDom = [&](const reflection::class_info& info, domain_master::context& dom) {
                    info.visit_functions([&](const reflection::function_info& func) {
                        if (&dom != &master.get_default_domain() && func.isStateless()) {
                            return;
                        }
                        reflection::bind_args args{
                            *vm,
                            info.className(),
                            func.name,
                            reinterpret_cast<reflection::bind_args::shared_state_t*>(&dom)
                        };
                        JC_log_full(IDebugLog::kLevel_DebugMessage,"skse callback: registerAllFunctions:\n class %s function %s", args.className.data(), args.functionName.data());
                        func.registrator(args);
                        // Replaces setting NoWait flag using SetFunctionFlags
                        vm->SetCallableFromTasklets(args.className.c_str(),
                                                    args.functionName.c_str(), true);
                    });
                };

                reflection::foreach_metaInfo_do(reflection::class_registry(), registerDom, master.get_default_domain());

                reflection::class_info amalgam = reflection::amalgamate_classes("dummy", reflection::class_registry());

                for (auto& domName : master.active_domain_names) {
                    auto& dom = master.get_or_create_domain_with_name(domName);
                    amalgam._className = domName;
                    registerDom(amalgam, dom);
                }
            });

            JC_log_full(IDebugLog::kLevel_DebugMessage, "skse callback: activating real api");
            jc_skse::set_real_api();

            return true;
        }

        bool Plugin_Load()
        {
            if (!m_serialization || !m_papyrus) {
                    JC_log_full(IDebugLog::LogLevel::kLevel_Error, "skse callback: Required SKSE interfaces are unavailable");
                    return false;
            }

            m_serialization->SetUniqueID(s_pluginHandle);
            JC_log_full(IDebugLog::kLevel_DebugMessage, "skse callback: Unique ID %08X", s_pluginHandle);

            m_serialization->SetRevertCallback(revert);
            m_serialization->SetSaveCallback(save);
            m_serialization->SetLoadCallback(load);
            m_serialization->SetFormDeleteCallback(delet);

            JC_log_full(
                IDebugLog::kLevel_DebugMessage, "skse callback: pluginHandle=%u serialization=%p version=%u",
                s_pluginHandle, m_serialization, m_serialization->Version()
            );

            bool res = m_papyrus->Register(registerAllFunctions);
            if (!res) {
                return false;
            }
            if (s_messaging) {
                s_messaging->RegisterListener(listene);
            }

            JC_log("skse callback: plugin loaded");

            return true;
        }

        private:
            const SKSE::SerializationInterface * m_serialization = nullptr;
            const SKSE::PapyrusInterface	     * m_papyrus     = nullptr;
            const static SKSE::MessagingInterface     * s_messaging;
            static SKSE::PluginHandle s_pluginHandle;
};

const SKSE::MessagingInterface *skse_callbacks::s_messaging = nullptr;
SKSE::PluginHandle skse_callbacks::s_pluginHandle = static_cast<SKSE::PluginHandle>(-1);
skse_callbacks *g_callbacks;

extern "C" [[maybe_unused]] __declspec(dllexport)
const SKSE::PluginDeclaration SKSEPlugin_Version({
    .Version = { JC_FILE_VERSION },
    .Name = plugin_name().c_str(),
    .Author = ""sv,
    .SupportEmail = ""sv,
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary,
    .MinimumSKSEVersion = REL::Version{ 0, 0, 0, 0 }
});

extern "C" [[maybe_unused]] __declspec(dllexport)
bool SKSEPlugin_Query(
    SKSE::QueryInterface* a_skse,
    SKSE::PluginInfo* a_info)
{
    if (!a_info) {
        return false;
    }

    a_info->infoVersion = SKSE::PluginInfo::kVersion;
    static const std::string pluginName{collections::plugin_name()};
    a_info->name = pluginName.c_str();
    a_info->version =
        static_cast<std::uint32_t>(
            SKSEPlugin_Version.GetVersion().pack());
    JC_log_full(IDebugLog::kLevel_DebugMessage, "Query: %s %s", a_info->name, JC_VERSION_STR);

    return true;
}

SKSEPluginLoad(const SKSE::LoadInterface *a_skse)
{
    gLog.OpenRelative(
            CSIDL_MYDOCUMENTS,
            (std::string(skse_logs()) + std::string(plugin_name()) + ".log").c_str()
        );
    gLog.SetPrintLevel(IDebugLog::kLevel_Error);
    gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

    // Logging of default spdlog output into a proper logfile.
    // Known Bug: Paths are incorrect, Skyrim Root is missing
    // Do not delete, needed for hard-to-reach spdlog output

    auto logger = spdlog::basic_logger_mt(
        "JContainersDebug",
        (std::string(skse_logs()) + std::string(plugin_name()) + "CL.log").c_str(),
        true
        );

    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    spdlog::set_default_logger(logger);

    logger->info("Loading of JContainers commenced: {}", plugin_name());
    spdlog::default_logger()->flush();

    JC_log_full(IDebugLog::LogLevel::kLevel_DebugMessage,"skse callback: Loading JContainers Plugin");

    auto pluginData = SKSE::PluginVersionData::GetSingleton();

    std::string pname(pluginData->GetPluginName());
    JC_log_full(IDebugLog::LogLevel::kLevel_DebugMessage,"Name: %s, Version %s", pname.c_str(), pluginData->GetPluginVersion().string().c_str());

    // SKSE::Init does the check, but no return value
    if (!a_skse) {
        JC_log_full(IDebugLog::LogLevel::kLevel_Error, "skse callback: Safety check for SKSE::Init failed - no load interface");
        return false;
    }
    SKSE::Init(a_skse);

    JC_log_full(IDebugLog::kLevel_Message, "%s Version %s", plugin_name().data(), JC_VERSION_STR);

    // No local variable (destroyed in unload)
    // any type of smart pointer is removed after return
    g_callbacks = new skse_callbacks();

    bool res = true; // Do not set to false!
#ifndef JC_SKSE_VR
    res = g_callbacks->SKSEQuery(a_skse);
#endif
    if (res) {
        res = g_callbacks->Plugin_Load();
    } else {
        JC_log_full(IDebugLog::LogLevel::kLevel_Error,"skse callback: SKSEPlugin Query failed");
    }

    JC_log_full(IDebugLog::LogLevel::kLevel_DebugMessage,"skse callback: PLugin Load finished with result %s", (res ? "success":"error"));

    return res;
}

}


