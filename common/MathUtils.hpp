#pragma once

#include <cstdint>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

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

template <typename T>
struct Extremum
{
    T value{};
    std::size_t index = 0;
};

template <typename T>
Extremum<T> min(const std::vector<T> &values)
{
    if (values.empty())
    {
        throw std::runtime_error("min_with_index requires non-empty input");
    }

    Extremum<T> result{values.front(), 0};
    for (std::size_t i = 1; i < values.size(); ++i)
    {
        const T &v = values[i];
        if (v < result.value)
        {
            result.value = v;
            result.index = i;
        }
    }
    return result;
}

template <typename T>
Extremum<T> max(const std::vector<T> &values)
{
    if (values.empty())
    {
        throw std::runtime_error("max_with_index requires non-empty input");
    }

    Extremum<T> result{values.front(), 0};
    for (std::size_t i = 1; i < values.size(); ++i)
    {
        const T &v = values[i];
        if (v > result.value)
        {
            result.value = v;
            result.index = i;
        }
    }
    return result;
}

template <typename Iterator>
Extremum<typename std::iterator_traits<Iterator>::value_type>
min(Iterator begin, Iterator end)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    if (begin == end)
    {
        throw std::runtime_error("min_with_index requires non-empty range");
    }

    Extremum<T> result{*begin, 0};
    std::size_t idx = 0;
    for (Iterator it = begin; it != end; ++it, ++idx)
    {
        if (*it < result.value)
        {
            result.value = *it;
            result.index = idx;
        }
    }
    return result;
}

template <typename Iterator>
Extremum<typename std::iterator_traits<Iterator>::value_type>
max(Iterator begin, Iterator end)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    if (begin == end)
    {
        throw std::runtime_error("max_with_index requires non-empty range");
    }

    Extremum<T> result{*begin, 0};
    std::size_t idx = 0;
    for (Iterator it = begin; it != end; ++it, ++idx)
    {
        if (*it > result.value)
        {
            result.value = *it;
            result.index = idx;
        }
    }
    return result;
}

} // namespace common::math
