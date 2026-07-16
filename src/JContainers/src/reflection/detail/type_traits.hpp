#pragma once

// To use this include implementation, add "bind_traits" as dependency
// Adding it here leads to circular include dependencies

#include "common/ITypes.h"
#include "reflection/reflection.h"
#include "reflection/tes_binding.h"
#include "skse/string.h"

namespace reflection {  namespace binding {

    template<>
        function_parameter type_info<void>() { return function_parameter_make("void", nullptr); }

    template<>
        function_parameter type_info<bool>() { return function_parameter_make("Bool", nullptr); }

    template<>
        function_parameter type_info<skse::string_ref>() { return function_parameter_make("String", nullptr); }

    template<> 
        function_parameter type_info< const char* >() { return function_parameter_make("String", nullptr); }

    template<>
        function_parameter type_info< Float32 >() { return function_parameter_make("Float", nullptr); }
    template<> 
        function_parameter type_info< SInt32 >() { return function_parameter_make("Int", nullptr); }
    template<>
        function_parameter type_info< UInt32 >() { return function_parameter_make("Int", "object"); }
    template<>
        function_parameter type_info< RE::TESForm * >() { return function_parameter_make("Form", nullptr); }
    template<>
        function_parameter type_info< RE::BGSListForm * >() { return function_parameter_make("FormList", nullptr); }
}
}
