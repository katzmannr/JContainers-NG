#include <SKSE/SKSE.h>

#include "RE/B/BSCoreTypes.h"
#include "RE/C/ConsoleLog.h"
#include "RE/F/FormTypes.h"
#include "RE/T/TESDataHandler.h"
#include "RE/T/TESForm.h"
#include "SkyrimVRESLAPI.h"
#include "common/IDebugLog.h"

#include <gtest/gtest.h>

extern SKSE::detail::SKSESerializationInterface* g_serialization;

namespace jc_skse
{

namespace
{

//--------------------------------------------------------------------------------------------------

/// Internal interface to follow on, same meaning as in the jc_skse.h
struct skse_api
{
    virtual std::optional<std::uint32_t> form_from_file (std::string_view const& name, std::uint32_t form) = 0;

    virtual std::optional<std::string_view> loaded_mod_name (std::uint8_t ndx) = 0;
    virtual std::optional<std::string_view> loaded_light_mod_name (std::uint16_t ndx) = 0;

    virtual RE::FormID resolve_handle (RE::FormID handle) = 0;
    virtual RE::TESForm* lookup_form (RE::FormID handle) = 0;

    virtual bool try_retain_handle (RE::FormID handle) = 0;
    virtual void release_handle (RE::FormID handle) = 0;

    virtual void console_print (const char * fmt, const va_list& args) = 0;
};

//--------------------------------------------------------------------------------------------------

/// Fake (for testing) API implementation
struct fake_api : public skse_api
{
    const std::string_view dict {
        "\0A\0B\0C\0D\0E\0F\0G\0H\0I\0J\0K\0L\0M\0N\0O\0P\0Q\0R\0S\0T\0U\0V\0W\0X\0Y\0Z", 53 };

    std::optional<std::string_view> loaded_mod_name (std::uint8_t ndx) override
    {
        if (auto n = dict.find (char (ndx)); n != std::string_view::npos)
            return &dict[n + !ndx];
        return std::nullopt;
    }

    std::optional<std::string_view> loaded_light_mod_name (std::uint16_t ndx) override
    {
        return loaded_mod_name (std::uint8_t (ndx));
    }

    std::optional<std::uint32_t> form_from_file (std::string_view const& name, std::uint32_t form) override
    {
        if (name.empty () || dict.find (name.front ()) == std::string_view::npos)
            return std::nullopt;
        return std::make_optional ((uint32_t (name.front ()) << 24) | (0x00ffffffu & form));
    }

    RE::FormID resolve_handle (RE::FormID handle) override { return handle; }

    RE::TESForm* lookup_form (RE::FormID) override
    {
        JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse FAKE lookupById");
        static char blob[sizeof(RE::TESForm)] = { '\0' };
        return reinterpret_cast<RE::TESForm*> (&blob);
    }

    bool try_retain_handle (RE::FormID) override { return true; }

    void release_handle (RE::FormID) override {}

    void console_print (const char*, const va_list&) override {}

};

TEST (skseAPI, testModnameFromIndex)
{
    fake_api t;

    EXPECT_EQ (t.loaded_mod_name ('Z'), "Z");
    EXPECT_EQ (t.loaded_light_mod_name ('A'), "A");

    EXPECT_FALSE (t.loaded_mod_name ('|'));
    EXPECT_FALSE (t.loaded_mod_name ('a'));
    EXPECT_FALSE (t.loaded_light_mod_name ('|'));
    EXPECT_FALSE (t.loaded_light_mod_name ('a'));
}

//--------------------------------------------------------------------------------------------------

/// Used to silence at run-time calls to SKSE (explain why?)
struct silent_api : public skse_api
{
    std::optional<std::uint32_t> form_from_file (std::string_view const&, std::uint32_t) override { return 0; }
    std::optional<std::string_view> loaded_mod_name (std::uint8_t) override { return ""; }
    std::optional<std::string_view> loaded_light_mod_name (std::uint16_t) override { return ""; }
    RE::FormID resolve_handle (RE::FormID) override { return 0; }
    RE::TESForm* lookup_form (RE::FormID) override {
        JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse SILENT lookupById");
        return nullptr;
    }
    bool try_retain_handle (RE::FormID) override { return true; }
    void release_handle (RE::FormID) override {}
    void console_print (const char*, const va_list&) override {}
};

//--------------------------------------------------------------------------------------------------

/// Actual wrapper around thin calls to SKSE
struct real_api : public skse_api
{
    std::optional<std::uint32_t> form_from_file (std::string_view const& name, std::uint32_t form) override
    {
        JC_log_full(IDebugLog::LogLevel::kLevel_DebugMessage,"jc_skse REAL form_from_file: view name %s, form 0x%X", name.data(), form);
        using namespace std;
        RE::TESDataHandler* p = RE::TESDataHandler::GetSingleton ();
        if (REL::Module::IsVR())
        {
            if (g_SkyrimVRESLInterface)
            {
                if (RE::TESFile const* mi = SkyrimVRESLPluginAPI::LookupAllLoadedModByName (string (name).c_str ()))
                {
                    auto retval = make_optional (SkyrimVRESLPluginAPI::GetFullFormID(mi, form));
                    return retval;
                }
            }
            else
            {
                RE::TESDataHandler* p = RE::TESDataHandler::GetSingleton ();
                if (RE::TESFile const* mi = p->LookupModByName (string (name).c_str ()))
                {
                    auto retval = p->LookupFormID(form, name);  //auto retval = make_optional (mi->GetFormID (form));
                    return retval;
                }
            }
        }
        else {
            RE::TESDataHandler* p = RE::TESDataHandler::GetSingleton ();
            if (RE::TESFile const* mi = p->LookupModByName (string (name).c_str ()))
            {
                auto retval = p->LookupFormID(form, name); // make_optional (mi->GetFormID (form));
                return retval;
            }
        }
        return std::nullopt;
    }

    /// Question: order in *Mods list is considered as modIndex or modLighIndex?
    std::optional<std::string_view> loaded_mod_name (std::uint8_t i) override
    {
        JC_log_full(IDebugLog::LogLevel::kLevel_DebugMessage,"jc_skse REAL loaded_mod_name");

        RE::TESDataHandler* p = RE::TESDataHandler::GetSingleton ();
        if (!REL::Module::IsVR())
        {
            if (i < p->GetLoadedModCount())
            {
                return p->GetLoadedMods()[i]->GetFilename();
            }
        }
        else if (i < p->GetLoadedModCount())
        {
            return p->GetLoadedMods()[i]->GetFilename();
        }
        return std::nullopt;
    }

    std::optional<std::string_view> loaded_light_mod_name (std::uint16_t i) override
    {
        JC_log_full(IDebugLog::LogLevel::kLevel_DebugMessage,"jc_skse REAL loaded_light_mod_name");

        if (REL::Module::IsVR())
        {
            if (g_SkyrimVRESLInterface)
            {
                const SkyrimVRESLPluginAPI::TESFileCollection* fileCollection = g_SkyrimVRESLInterface->GetCompiledFileCollection();
                if (i < fileCollection->smallFiles.size())
                {
                    RE::TESFile* smallFile = nullptr;
                    smallFile = fileCollection->smallFiles[i];
                    return (smallFile != nullptr ? smallFile->GetFilename() : "");
                }
            }
            else
            {
                JC_log_full(IDebugLog::LogLevel::kLevel_Warning, "jc_skse Attempted to fetch a light plugin name in VR, but VR ESL support is not  present!");
            }
        } else {
            RE::TESDataHandler* p = RE::TESDataHandler::GetSingleton ();
            if (i < p->GetLoadedModCount())
                return p->GetLoadedMods()[i]->GetFilename();
        }
        return std::nullopt;
    }

    RE::FormID resolve_handle (RE::FormID id) override
    {
        JC_log_full(IDebugLog::LogLevel::kLevel_DebugMessage,"jc_skse REAL resolve_handle");

        // Already resolved ? Just return the id
        // return g_serialization->ResolveFormId (old_id, &new_id) ? static_cast<FormId> (new_id) : FormId::Zero;
        // not resolved
        // return RE::TESForm::LookbyID(id) -> does return the Form, you could get the form with RE::TESDataHandler again
        return id;
    }

    RE::TESForm* lookup_form (RE::FormID id) override
    {   
        RE::TESForm *form;
        JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse REAL lookupById: id is %d ", id);
        form = RE::TESForm::LookupByID(id);
        if (form!=nullptr) {
            JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse REAL lookupById: Form Type %s, Id %d is 0x%X",RE::FormTypeToString(form->GetFormType()).data(), form->GetFormID(), form);
        } else {
            JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse REAL lookupById: Invalid Form Id");
        }
        return form;
    }

    bool try_retain_handle (RE::FormID id) override
    {
        JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse REAL try_retain_handle: Form Id %d",id);
        auto form = lookup_form (id);
        if (!form)
            return false;

        // Now done internally in CommonLibNG-SE
        // auto policy = *g_objectHandlePolicy;
        // auto handle = policy->Create (form->formType, form);
        // if (handle == policy->GetInvalidHandle ())
        //     return false;

        // policy->AddRef (handle);
        return true;
    }

    void release_handle (RE::FormID id) override
    {
        JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse REAL release_handle: Form Id %d",id);
        // Now done internally in CommonLibNG-SE
        // auto form = lookup_form (id);
        // if (!form)
        //     return;

        // auto policy = *g_objectHandlePolicy;
        // auto handle = policy->Create (form->formType, form);
        // if (handle != policy->GetInvalidHandle ())
        //     policy->Release (handle);
    }

    void console_print (const char * fmt, const va_list& args) override
    {
        RE::ConsoleLog console;
        // Hangs the game, reason not known
        //console.Print(fmt, args);
    }
};

//--------------------------------------------------------------------------------------------------

fake_api g_fake_api;
real_api g_real_api;
silent_api g_silent_api;
skse_api* g_current_api = &g_fake_api;

} // anonymous namespace

//--------------------------------------------------------------------------------------------------

void set_real_api ()
{
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: set_real_api");
    auto real_api = &g_real_api;
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: set_real_api: Assigned simple variable: %p",real_api);
    g_current_api = real_api;
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: real api activated. Good luck");
}

void set_fake_api ()
{
   JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: set_fake_api");
   g_current_api = &g_fake_api;
}

void set_silent_api ()
{
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: set_silent_api");
    g_current_api = &g_silent_api;
}

RE::FormID resolve_handle (RE::FormID handle)
{
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: resolve_handle: %d", handle);
    return g_current_api->resolve_handle (handle);
}

RE::TESForm* lookup_form (RE::FormID handle)
{
    RE::TESForm *form = nullptr;
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse lookup_form: Handle is %d ", handle);
    if (handle == 0) {
        JC_log_full(IDebugLog::kLevel_Warning,"jc_skse lookup_form: Invalid handle 0");
    } else {
        form = g_current_api->lookup_form (handle);
        JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse lookup_form: Form Type %s, Id %d is 0x%X",RE::FormTypeToString(form->GetFormType()).data(), form->GetFormID(), form);
        if (g_current_api != &g_real_api) // Prevent crash on new game
        {
            form = nullptr; // This may cause test failures
        }
    }
    return form;
}

std::optional<std::uint32_t> form_from_file (std::string_view const& name, std::uint32_t form)
{
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: form_from_file: %s", name.data());
    return g_current_api->form_from_file (name, form);
}

std::optional<std::string_view> loaded_mod_name (std::uint8_t idx)
{
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: loaded_mod_name: idx %d", idx);
    return g_current_api->loaded_mod_name (idx);
}

std::optional<std::string_view> loaded_light_mod_name (std::uint16_t idx)
{
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: loaded_light_mod_name: idx %d", idx);
    return g_current_api->loaded_light_mod_name (idx);
}

void console_print (const char* fmt, const va_list& args)
{
    g_current_api->console_print (fmt, args);
}

void console_print (const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    console_print (fmt, args);
    va_end (args);
}

bool try_retain_handle (RE::FormID handle)
{
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: try_retain_handle: %d", handle);
    return g_current_api->try_retain_handle (handle);
}

void release_handle (RE::FormID handle)
{
    JC_log_full(IDebugLog::kLevel_DebugMessage,"jc_skse: release_handle: %d", handle);
    g_current_api->release_handle (handle);
}

//--------------------------------------------------------------------------------------------------

} // namespace skse

