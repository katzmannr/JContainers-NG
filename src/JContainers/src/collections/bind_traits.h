#pragma once

#include "common/IDebugLog.h"
#include "typedefs.h"
#include "util/stl_ext.h"
#include "reflection/tes_binding.h"
#include "collections.h"
#include "context.h"
#include <SKSE/SKSE.h>
#include <RE/T/TESForm.h>
#include "skse/jc_skse.h"
#include "domains/domain_master.h"

namespace reflection { namespace binding {

    using namespace collections;

    template<class T = object_base>
    struct ObjectConverter {

        typedef HandleT tes_type;

        static HandleT convert2Tes(object_base* obj) {
            if (obj == nullptr) {
                JC_log_full(IDebugLog::kLevel_Warning,"convert2Tes: Invalid object.");
            }
            return (HandleT)(obj ? obj->uid() : Handle::Null);
        }

        static object_stack_ref_template<T> convert2J (HandleT hdl, tes_context& ctx)
        {
            auto ref = ctx.getObjectRefOfType<T> ((Handle) hdl);
            if (!ref && hdl != util::to_integral (Handle::Null))
                JC_log_full(IDebugLog::kLevel_Warning,"convert2J: Access to non-existing object with id 0x%X (%d)", hdl, hdl);
            return ref;
        }
    };

    template<> struct GetConv < object_stack_ref > : ObjectConverter<>{};

    template<> struct GetConv < object_base* > : ObjectConverter<>{};
    template<> struct GetConv < array* > : ObjectConverter< array >{};
    template<> struct GetConv < map* > : ObjectConverter< map >{};
    template<> struct GetConv < form_map* > : ObjectConverter< form_map >{};
    template<> struct GetConv < integer_map* > : ObjectConverter < integer_map >{};

    //////////////////////////////////////////////////////////////////////////

    template<> struct GetConv < Handle > : StaticCastValueConverter<Handle, HandleT> {};

    //////////////////////////////////////////////////////////////////////////

    template<> struct GetConv < forms::form_ref > {
        typedef RE::TESForm* tes_type;
        static RE::TESForm* convert2Tes(const forms::form_ref& id) {
            JC_log_full(IDebugLog::kLevel_DebugMessage,"tes_type convert2Tes: id is %d ", id.get());
            auto form = jc_skse::lookup_form(id.get());
            if (form == nullptr || id.get() == 0) {
                JC_log_full(IDebugLog::kLevel_Warning,"tes_type convert2Tes: Form not found or invalid form. form id %d", id.get());
                form = nullptr;
            } else {
                JC_log_full(IDebugLog::kLevel_DebugMessage,"tes_type convert2Tes: Form Id %d",form->GetFormID());
            }
            return form;
        }
        static forms::form_ref convert2J(const RE::TESForm* form, tes_context& ctx) {
            if (form == nullptr) {
                JC_log_full(IDebugLog::kLevel_Warning,"tes_type convert2J: Form does not exist on context %s.", ctx.write_to_string().data());
            } else {
                JC_log_full(IDebugLog::kLevel_DebugMessage,"tes_type convert2J: Form Id %d",form->GetFormID());
            }
            return make_weak_form_id(form, ctx);
        }
        template<class Any>
        static const forms::form_ref convert2J(const RE::TESForm* form, const Any&) {
            if (form == nullptr) {
                JC_log_full(IDebugLog::kLevel_Warning,"Any convert2J: Form does not exist.");
                return forms::form_ref{};
            } else {
                JC_log_full(IDebugLog::kLevel_DebugMessage,"Any convert2J: Form Id %d",form->GetFormID());
            }
            return forms::form_ref(form->GetFormID(), domain_master::master::instance().get_form_observer());
        }
    };

    template<> struct GetConv < forms::form_ref_lightweight > {
        typedef RE::TESForm* tes_type;
        static RE::TESForm* convert2Tes(const forms::form_ref_lightweight& id) {
            JC_log_full(IDebugLog::kLevel_DebugMessage,"convert2Tes lw: id is %d ", id.get());
            auto form = jc_skse::lookup_form(id.get());
            if (form == nullptr || id.get() == 0) {
                JC_log_full(IDebugLog::kLevel_Warning,"convert2Tes lw: Form for lightweight not found or invalid form. form id %d", id.get());
                form = nullptr;
            } else {
                JC_log_full(IDebugLog::kLevel_DebugMessage,"convert2Tes lw: Form Id %d",form->GetFormID());
            }
            return form;
        }
        static forms::form_ref_lightweight convert2J(const RE::TESForm* form ,tes_context& ctx) {
            if (form == nullptr) {
                JC_log_full(IDebugLog::kLevel_Warning,"convert2J lw: Form for lightweight does not exit on context %s.", ctx.write_to_string().data());
                return forms::form_ref_lightweight{};
            } else {
                JC_log_full(IDebugLog::kLevel_DebugMessage,"convert2J lw: Form Id %d",form->GetFormID());
            }
            return make_lightweight_form_ref(form, ctx);
        }
    };

    template<class T>
    struct GetConv<rbArray<T>>
    {
        using tes_type = RE::BSScript::reference_array<T>;

        static rbArray<T> convert2J(
            RE::BSScript::reference_array<T>&& arr,
            const collections::tes_context&)
        {
            return rbArray<T>{ std::move(arr) };
        }

        static RE::BSScript::reference_array<T> convert2Tes(rbArray<T>&& arr)
        {
            return std::move(arr.arr);
        }
    };

}
}
