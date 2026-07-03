#pragma once

#include <assert.h>

#include <boost/serialization/split_member.hpp>
#include <boost/optional.hpp>

#include "common/ITypes.h"
#include "common/IDebugLog.h"
#include <skse/skse.h>

#include "collections/collections_types.h"

namespace collections {

    template<class R, class Collection, class F, class ...Args>
    inline R perform_on_object_and_return(Collection& container, F&& func, Args&&... args) {
        switch (container.type()) {
        case array::TypeId:
            return func(container.template as_link<array>(), std::forward<Args>(args)...);
        case map::TypeId:
            return func(container.template as_link<map>(), std::forward<Args>(args)...);
        case form_map::TypeId:
            return func(container.template as_link<form_map>(), std::forward<Args>(args)...);
        case integer_map::TypeId:
            return func(container.template as_link<integer_map>(), std::forward<Args>(args)...);
        default:
            assert(false);
            noreturn_func();
            break;
        }
    }

    template<class F, class Collection, class ...Args>
    inline void perform_on_object(Collection& container, F&& func, Args&&... args) {
        switch (container.type()) {
        case array::TypeId:
            func(container.template as_link<array>(), std::forward<Args>(args)...);
            break;
        case map::TypeId:
            func(container.template as_link<map>(), std::forward<Args>(args)...);
            break;
        case form_map::TypeId:
            func(container.template as_link<form_map>(), std::forward<Args>(args)...);
            break;
        case integer_map::TypeId:
            func(container.template as_link<integer_map>(), std::forward<Args>(args)...);
            break;
        default:
            assert(false);
            break;
        }
    }
}
