#pragma once

#include <vector>

#include <SKSE/SKSE.h>
// Using only SKSE.h is NOT sufficient for NativeFunction and PackUnpack
#include <RE/N/NativeFunction.h>
#include <RE/P/PackUnpack.h>
#include "collections/collections_types.h"
#include "collections/context.h"
#include "reflection/reflection.h"
#include "common/ITypes.h"
#include "skse/string.h"

namespace reflection { namespace binding {

    // Wrapper replacement for old VMArray style arrays
    template <typename T>
    class rbArray
    {
    public:
        rbArray(RE::BSScript::reference_array<T>&& a)
            : arr(std::move(a))
        {}
        rbArray(rbArray&&) = default;
        rbArray& operator=(rbArray&&) = default;

        rbArray(const rbArray&) = delete;
        rbArray& operator=(const rbArray&) = delete;

        // Ownership is transferred in GetConv!
        RE::BSScript::reference_array<T> arr;
        UInt32 Length() const
        {
            return static_cast<UInt32>(arr.size());
        }
        void Get(T* dst, UInt32 idx)
        {
            *dst = arr[idx];
        }
        void Set(const T* src, UInt32 idx)
        {
            arr[idx] = *src;
        }
    };

    // traits placeholders
    template<class JType>
    struct IdentityConverter {
        typedef JType tes_type;

        template<class State>
        static const tes_type& convert2J(const tes_type& val, const State&) {
            return val;
        }

        static const tes_type& convert2Tes(const tes_type& val) {
            return val;
        }
    };

    template<class JType, class TesType>
    struct StaticCastValueConverter {
        typedef TesType tes_type;

        template<class State>
        static JType convert2J(const TesType& val, const State&) {
            return static_cast<JType>(val);
        }

        static TesType convert2Tes(const JType& val) {
            return static_cast<TesType>(val);
        }
    };

    struct StringConverter {
        using tes_type = skse::string_ref;

        template<class State>
        static const char* convert2J(const skse::string_ref& str, const State&) {
            return str.c_str();
        }

        template<class AnyString>
        static skse::string_ref convert2Tes(const AnyString& str) {
            return skse::string_ref(str);
        }
    };

    template<class JType> struct GetConv : IdentityConverter < JType > {
        //typedef ValueConverter<TesType> Conv;
    };

    template<> struct GetConv<void> {
        using tes_type = void;
    };

    template<> struct GetConv<const char*> : StringConverter{};
    template<> struct GetConv<std::string> : StringConverter{};

    template<> struct GetConv<int32_t> : StaticCastValueConverter<int32_t, SInt32>{};
    template<> struct GetConv<uint32_t> : StaticCastValueConverter<uint32_t, UInt32>{};

    template <class T>
    struct tes_type;

    template <>
    struct tes_type<int> {
        using type = std::int32_t;
    };

    template <>
    struct tes_type<float> {
        using type = float;
    };

    template <>
    struct tes_type<bool> {
        using type = std::int32_t;
    };

    template<>
    struct tes_type<skse::string_ref> {
        using type = RE::BSFixedString;
    };

    template <>
    struct tes_type<std::string> {
        using type = RE::BSFixedString;
    };

    template <class T>
    using tes_type_t = typename tes_type<T>::type;

    template <class R>
    tes_type_t<R> to_tes(R&& r)
    {
        return GetConv<R>::convert2Tes(std::forward<R>(r));
    }

    //////////////////////////////////////////////////////////////////////////

    template<class T>
    function_parameter type_info();

    template<class T> struct j2Str {
        static function_parameter typeInfo() {
            return reflection::binding::type_info<T>();
        }
    };

    template<class T> struct j2Str < rbArray<T> > {
        static function_parameter typeInfo() {
            std::string str(j2Str<T>::typeInfo().tes_type_name);
            str += "[]";
            function_parameter info = { str, "values" };
            return info;
        }
    };

    template<class T>
    struct j2Str<RE::BSScript::reference_array<T>>
    {
        static function_parameter typeInfo()
        {
            std::string str(j2Str<T>::typeInfo().tes_type_name);
            str += "[]";

            function_parameter info = {
                str,
                "values"
            };
            return info;
        }
    };

    template<class T> struct j2Str < std::vector<T> > : j2Str < rbArray<T> > {};

    //////////////////////////////////////////////////////////////////////////

    template<class T>
    using remove_cref = typename std::remove_const<typename std::remove_reference<T>::type>::type;

    template<class T>
    using get_converter = GetConv< remove_cref<T> >;

    template<class T>
    using convert_to_tes_type = typename get_converter<T>::tes_type;

    // Template monster, proxy class that:
    // - adapts my internal types to native Papyrus types and vica versa
    // - generates native Papyrus function
    // - holds function meta-info, like @parameter_info
    template <typename T> struct proxy;
    template <typename T> struct state_proxy;

    struct no_state {};

    template<typename>
    struct function_traits;

    // Function trait for function (auto func) inspection
    template<typename R, typename... Args>
    struct function_traits<R(*)(Args...)>
    {
        using return_type = R;

        template<std::size_t I>
        using arg = std::tuple_element_t<I, std::tuple<Args...>>;

        static constexpr std::size_t argc = sizeof...(Args);
    };

    template<typename F>
    struct first_argument
    {
        using type = void;
    };

    template<typename R, typename A0, typename... A>
    struct first_argument<R(*)(A0, A...)>
    {
        using type = A0;
    };

    template<class Derived, class R, class State = no_state, class... Params>
    struct proxy_common
    {
        static std::vector<type_info_func> parameter_info() {
            return {
                &j2Str< convert_to_tes_type<R> >::typeInfo,
                &j2Str< convert_to_tes_type<Params> >::typeInfo ...
            };
        }

        static constexpr bool is_stateless =
            std::is_same_v<State, no_state>;

        // subtype @magick to workaround some msvc2013 bug
        template<auto func>
        struct magick_impl {

            using return_type = R;
            using base = proxy_common;

            using traits = function_traits<decltype(func)>;

            static auto func_ptr() -> decltype(func) {
                return func;
            }

            using first = first_argument<decltype(func)>::type;

            static_assert(std::is_reference_v<first> || is_stateless);
            static_assert(
                std::is_same_v<
                    std::remove_reference_t<first>,
                    collections::tes_context> || is_stateless);

            struct runtime_callback {
                State& _callbackState;

                runtime_callback(State& state)
                    : _callbackState(state)
                {}

                convert_to_tes_type<R> operator() (
                    RE::StaticFunctionTag* tag,
                    convert_to_tes_type<Params>... params)
                    {
                        if constexpr (std::is_void_v<R>) {
                            if constexpr (std::is_same_v<State, no_state>) {
                                func(get_converter<Params>::convert2J(std::move(params), tag) ...);
                            } else {
                                func(_callbackState, get_converter<Params>::convert2J(std::move(params), _callbackState) ...);
                            }
                            return;
                        } else {
                            using first = typename first_argument<decltype(func)>::type;

                            static constexpr bool first_is_state =
                                std::is_same_v<
                                    std::remove_cvref_t<first>,
                                    std::remove_cvref_t<State>>;
                            if constexpr (std::is_void_v<R>)
                            {
                                if constexpr (std::is_same_v<State, no_state>) {
                                    func(get_converter<Params>::convert2J(params, tag) ...);
                                } else {
                                    func(_callbackState, get_converter<Params>::convert2J(std::move(params), _callbackState) ...);
                                }
                                return;    // OK for void
                            }
                            else // callback has non-void return value
                            {
                                if constexpr (std::is_same_v<State, no_state>) {
                                    return GetConv<R>::convert2Tes(
                                        func(
                                            get_converter<Params>::convert2J(std::move(params), tag) ...
                                            )
                                        );
                                }
                                else {
                                    return GetConv<R>::convert2Tes(
                                        func(
                                            _callbackState,
                                            get_converter<Params>::convert2J(std::move(params), _callbackState) ...
                                            )
                                        );
                                }
                            }
                        }
                    }
            };

            inline static std::unique_ptr<runtime_callback> callback;

            static void initialize(State& state, const char* className, const char*funcName)
            {
                if( callback ) {
                    JC_log_full(IDebugLog::kLevel_Error, "tes_binding: Function registered twice %s.%s (%p)",className, funcName, reinterpret_cast<void*>(&tes_func));
                    //std::string output = std::format("Function {}.{} registered twice", className, funcName);
                    //assert(!callback && output.c_str());
                    return;
                }
                callback = std::make_unique<runtime_callback>(state);
            }

            // Revive interface for external access to callback
            static convert_to_tes_type<R> tes_func(
                RE::StaticFunctionTag* tag,
                convert_to_tes_type<Params>... params)
            {
                assert(callback && "runtime callback not initialized");
                // reference_array is move-only in CommonLib; do not copy it
                return (*callback)(tag, std::move(params)...);
            }

            static void bind(const bind_args& args)
            {
                auto& state = *reinterpret_cast<State*>(args.shared_state);
                initialize(state, args.className.c_str(), args.functionName.c_str());
                args.vm.RegisterFunction(
                    args.functionName.c_str(),
                    args.className.c_str(),
                    &tes_func
                );
            }
        };
    };

    template <class R, class... Params>
    struct proxy<R(*)(Params ...)>
        : proxy_common<proxy<R(*)(Params...)>, R, no_state, Params...>
    {
        using base = proxy;
        using common = proxy_common<proxy<R(*)(Params...)>, R, no_state, Params...>;
        template<R(*func)(Params...)>
        struct magick : common::template magick_impl<func> {};
    };

    template <class R, class State, class... Params>
    struct state_proxy<R(*)(State&, Params ...)>
        : proxy_common<proxy<R(*)(Params...)>, R, State, Params...>
    {
        using base = state_proxy;
        using common = proxy_common<proxy<R(*)(State&, Params...)>, R, State, Params...>;
        template<R(*func)(State&, Params...)>
        struct magick : common::template magick_impl<func> {};
    };

#define CONCAT(x, y) CONCAT1 (x, y)
#define CONCAT1(x, y) x##y

    // MSVC2012 bug workaround
    template <typename T> T msvc_identity(T);

    struct name_setter {
        explicit name_setter(class_info& info, const char* className) {
            info._className = className;
        }
    };

#define REGISTER_TES_NAME(ScriptTesName)  \
    ::reflection::binding::name_setter _name_setter{ metaInfo, ScriptTesName };

    struct function_registree {

        template<class Binder, class String2>
        inline function_registree(class_info& info,
            Binder,
            const char* funcname, const char* argument_names, const String2& comment)
        {
            using namespace ::reflection;

            static_assert( false == std::is_same<typename Binder::return_type, const char *>::value,
                "a trap for 'const char *' return types" );

            function_info metaF;
            metaF.registrator = &Binder::bind;
            metaF.param_list_func = &Binder::base::parameter_info;

            metaF.argument_names = (argument_names) ? (argument_names) : "";
            if constexpr (std::is_convertible_v<String2, function_info::comment_generator>) {
                metaF.setComment(static_cast<function_info::comment_generator>(comment));
            } else if constexpr (std::is_convertible_v<String2, const char*>) {
                metaF.setComment(comment);
            } else if constexpr (std::is_same_v<std::decay_t<String2>, std::string>) { // std::is_convertible_v<String2, std::string>
                metaF.setComment(comment.c_str());
            } else {
                static_assert(sizeof(String2) == 0, "Unsupported comment type");
                metaF.setComment("Unsuppported");
            }
            metaF.name = funcname;
            metaF.tes_func = &Binder::tes_func;
            metaF.c_func = static_cast<c_function>(Binder::func_ptr());
            metaF._stateless = Binder::base::is_stateless;

            info.addFunction(metaF);
        }
    };

#define REGISTERF REGISTERF_STATE
#define REGISTERF_STATELESS(func, _funcname, _args, _comment)\
    ::reflection::binding::function_registree CONCAT(_func_registree_, __LINE__){ \
        metaInfo,\
    ::reflection::binding::proxy<decltype(::reflection::binding::msvc_identity(&func))>::magick<&func>(), \
        _funcname, _args, _comment };

#define REGISTERF2(func, args, comment)     REGISTERF(func, #func, args, comment)
#define REGISTERF2_STATELESS(func, args, comment)     REGISTERF_STATELESS(func, #func, args, comment)

#define REGISTERF_STATE(func, _funcname, _args, _comment)\
    ::reflection::binding::function_registree CONCAT(_func_registree_, __LINE__){ \
        metaInfo, \
    ::reflection::binding::state_proxy<decltype(::reflection::binding::msvc_identity(&func))>::template magick<&func>(), \
        _funcname, _args, _comment \
};

    struct papyrus_textblock_setter {
        explicit papyrus_textblock_setter(class_info& info, const papyrus_text_block& text) {
            info.add_text_block(text);
        }
    };

#define REGISTER_TEXT(text) \
    ::reflection::binding::papyrus_textblock_setter CONCAT(_textblock_setter_, __LINE__){ metaInfo, ::reflection::papyrus_text_block(text) };


}
}
