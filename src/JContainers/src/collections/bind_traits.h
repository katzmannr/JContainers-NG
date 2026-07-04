#pragma once

#include "util/stl_ext.h"
#include "reflection/tes_binding.h"
#include "collections.h"
#include "context.h"
#include <SKSE/SKSE.h>
#include "skse/skse.h"

namespace reflection { namespace binding {

    using namespace collections;

    template<class T = object_base>
    struct ObjectConverter {

        typedef HandleT tes_type;

        static HandleT convert2Tes(object_base* obj) {
            return (HandleT)(obj ? obj->uid() : Handle::Null);
        }

        static object_stack_ref_template<T> convert2J (HandleT hdl, tes_context& ctx)
        {
            auto ref = ctx.getObjectRefOfType<T> ((Handle) hdl);
            if (!ref && hdl != util::to_integral (Handle::Null))
                JC_log ("Warning: access to non-existing object with id 0x%X (%d)", hdl, hdl);
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

    template<> struct GetConv < FormId > {
        typedef RE::TESForm* tes_type;
        static RE::TESForm* convert2Tes(FormId id) {
            return RE::TESForm::LookupByID((uint32_t)id);
        }
        template<class Any>
        static FormId convert2J(const RE::TESForm* form, const Any&) {
            return form ? (FormId)form->formID : FormId::Zero;
        }
    };

    /////////////////

    template<> struct GetConv < forms::form_ref > {
        typedef RE::TESForm* tes_type;
        static RE::TESForm* convert2Tes(const forms::form_ref& id) {
            return jc_skse::lookup_form(id.get());
        }
        static forms::form_ref convert2J(const RE::TESForm* form, tes_context& ctx) {
            return make_weak_form_id(form, ctx);
        }
    };

    template<> struct GetConv < forms::form_ref_lightweight > {
        typedef RE::TESForm* tes_type;
        static RE::TESForm* convert2Tes(const forms::form_ref_lightweight& id) {
            return jc_skse::lookup_form(id.get());
        }
        static forms::form_ref_lightweight convert2J(const RE::TESForm* form ,tes_context& ctx) {
            return make_lightweight_form_ref(form, ctx);
        }
    };

}
}
