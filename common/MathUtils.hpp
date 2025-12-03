#pragma once

#include <cstdint>
#include <numeric>
#include <type_traits>

namespace common::math
{
template <typename T>
constexpr T gcd(T a, T b)
{
    return std::gcd(a, b);
}

template <typename T>
constexpr T lcm(T a, T b)
{
    return std::lcm(a, b);
}

template <typename T>
constexpr T safe_mod(T value, T mod)
{
    value %= mod;
    if (value < 0)
    {
        value += mod;
    }
    return value;
}

template <typename Base, typename Exponent, typename Mod>
Base pow_mod(Base base, Exponent exp, Mod mod)
{
    static_assert(std::is_integral_v<Base> && std::is_integral_v<Exponent> && std::is_integral_v<Mod>,
                  "pow_mod expects integral types");
    if (mod == 1)
    {
        return 0;
    }
    Base result = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1)
        {
            result = (result * base) % mod;
        }
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

} // namespace common::math
