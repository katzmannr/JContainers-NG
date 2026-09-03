#pragma once
#include <type_traits>

// JContainers requires C++2020, the rest has moved on.
#if __cplusplus < 202302L && !defined(__cpp_lib_to_underlying)
namespace std {
template <class E>
constexpr auto to_underlying(E e) noexcept
    -> std::underlying_type_t<E>
{
    return static_cast<std::underlying_type_t<E>>(e);
}
}
#endif