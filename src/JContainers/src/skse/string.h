#pragma once

#include <string>
#include <SKSE/SKSE.h>
#include <RE/T/TypeTraits.h>
#include <RE/T/TypeInfo.h>
#include <RE/P/PackUnpack.h>

namespace skse
{
    class string_ref
    {
    public:
        string_ref() = default;

        explicit string_ref(const char* val) :
            _str(val)
        {}

        string_ref(const string_ref&) = default;
        string_ref(string_ref&&) = default;

        string_ref& operator=(const string_ref&) = default;
        string_ref& operator=(string_ref&&) = default;

        string_ref& operator=(const char* val)
        {
            _str = val;
            return *this;
        }

        operator const RE::BSFixedString&() const
        {
            return _str;
        }

        operator RE::BSFixedString() const
        {
            return _str;
        }

        // CommonLibSE internal representation of strings
        operator std::string_view() const noexcept
        {
            return std::string_view{_str.c_str(), _str.size()};
        }

        template<class Tr, class Alloc>
        explicit string_ref(const std::basic_string<char, Tr, Alloc>& str)
            : _str(str.c_str())
        {}

        template <class Tr, class Alloc>
        string_ref& operator=(const std::basic_string<char, Tr, Alloc>& str)
        {
            _str = str.c_str();
            return *this;
        }

        const char* c_str() const
        {
            return _str.c_str();
        }

        bool operator==(const string_ref& rhs) const
        {
            return _str == rhs._str;
        }

    private:
        RE::BSFixedString _str;
    };
}

// Inject our wrapper class for BSFixedString used for reflection and binding
template<>
struct RE::BSScript::GetRawType<skse::string_ref>
{
    constexpr RE::BSScript::TypeInfo::RawType operator()() const noexcept
    {
        return GetRawType<RE::BSFixedString>{}();
    }
};
