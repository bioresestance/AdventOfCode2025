#pragma once

#include <string>
#include <string_view>
#include <type_traits>

namespace common
{
namespace detail
{
template <typename T>
struct always_false : std::false_type
{
};

template <typename T>
std::string normalizeResult(T &&value)
{
    using Decayed = std::decay_t<T>;
    if constexpr (std::is_same_v<Decayed, std::string>)
    {
        return value;
    }
    else if constexpr (std::is_same_v<Decayed, std::string_view>)
    {
        return std::string(value);
    }
    else if constexpr (std::is_arithmetic_v<Decayed>)
    {
        return std::to_string(value);
    }
    else
    {
        static_assert(always_false<T>::value, "Unsupported result type for Advent of Code runner");
    }
}
}
} // namespace common
