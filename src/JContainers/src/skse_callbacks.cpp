#include <boost/iostreams/stream.hpp>
#include "RE/B/BSCoreTypes.h"
#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include "RE/I/IVirtualMachine.h"
#include "SKSE/Interfaces.h"
#include <shlobj.h>


#include <SKSE/SKSE.h>

#include "SkyrimVRESLAPI.h"

#include "common/ITypes.h"
#include "util/util.h"
#include "jc_interface.h"
#include "reflection/reflection.h"
#include "jcontainers_constants.h"

#include "collections/context.h"
#include "forms/form_observer.h"

#include "domains/domain_master.h"

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
            JC_log("couldn't get papyrus interface");
            return;
        }
        s_messaging = SKSE::GetMessagingInterface();
    }

    static void revert(SKSE::SerializationInterface *intfc) {
        util::do_with_timing("Revert", []() {
            jc_skse::set_silent_api();
            domain_master::master::instance().clear_state();
            jc_skse::set_real_api();
        });
    }

    static void save(SKSE::SerializationInterface * intfc) {

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
                    JC_log("Unable open JC record");
                }
            });
        }

        static void load(SKSE::SerializationInterface * intfc) {

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
                    if (static_cast<consts>(type) == consts::storage_chunk) {
                        break;
                    }
                }

                io::stream<skse_data_source> stream(skse_data_source(static_cast<consts>(type) == consts::storage_chunk ? intfc : nullptr));
                domain_master::master::instance().read_from_stream(stream);
            });
        }

        static void delet(RE::VMHandle handle) {
            domain_master::master::instance().get_form_observer().on_form_deleted((forms::FormHandle)handle);
        }

        static void listene(SKSE::MessagingInterface::Message* msg) {
            if (msg && msg->type == SKSE::MessagingInterface::kPostPostLoad) {
                s_messaging->Dispatch(jc::message_root_interface, (void *)&jc::root, sizeof(void*), nullptr);
                if (!REL::Module::IsVR())
                {
                    SkyrimVRESLPluginAPI::GetSkyrimVRESLInterface001(s_pluginHandle, s_messaging);
                    if (g_SkyrimVRESLInterface)
                    {
                        JC_log("SkyrimVRESL interface detected and initialized!");
                    }
                    else
                    {
                        JC_log("SkyrimVRESL interface is not present or has failed to be retrieved... ESL related functionality is disabled.");
                    }
                }
            }
        }

        static bool registerAllFunctions(RE::BSScript::IVirtualMachine *vm) {

            gLog.OpenRelative(CSIDL_MYDOCUMENTS, (std::string(skse_logs()) + std::string(plugin_name()) + ".log").c_str());
            gLog.SetPrintLevel(IDebugLog::kLevel_Error);
            gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

            // store plugin handle so we can identify ourselves later
            s_pluginHandle = SKSE::GetPluginHandle();

            // This old check could be useful in a rare case of multiple mixed version build
            // messaging && messaging->interfaceVersion >= SKSE::MessagingInterface::kVersion)

            JC_log("%s %s", plugin_name(), JC_VERSION_STR);

            jc_assert(vm);

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

            return true;
        }

        bool SKSEPlugin_Load()
        {
            m_serialization->SetUniqueID(s_pluginHandle);

            m_serialization->SetRevertCallback(revert);
            m_serialization->SetSaveCallback(save);
            m_serialization->SetLoadCallback(load);

            m_serialization->SetFormDeleteCallback(delet);

            m_papyrus->Register(registerAllFunctions);

            if (s_messaging) {
                s_messaging->RegisterListener(listene);
            }

            JC_log("plugin loaded");

            return true;
        }

        /// Since SKSE 2.3.1 it is not actually called, now CommonLibSSE-NG takes care of this part.
        // bool SKSEPlugin_Query (const SKSEInterface * skse, PluginInfo * info)

        private:
            const SKSE::SerializationInterface * m_serialization = nullptr;
            const SKSE::PapyrusInterface	     * m_papyrus     = nullptr;
            const static SKSE::MessagingInterface     * s_messaging;
            static SKSE::PluginHandle s_pluginHandle;
};

const SKSE::MessagingInterface *skse_callbacks::s_messaging = nullptr;
SKSE::PluginHandle skse_callbacks::s_pluginHandle = static_cast<SKSE::PluginHandle>(-1);

}


