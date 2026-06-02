#pragma once

#include <string>
#include <SKSE/SKSE.h>

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
