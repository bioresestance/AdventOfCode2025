#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace common::str
{
inline std::string trim_copy(std::string_view input)
{
    while (!input.empty() && std::isspace(static_cast<unsigned char>(input.front())))
    {
        input.remove_prefix(1);
    }
    while (!input.empty() && std::isspace(static_cast<unsigned char>(input.back())))
    {
        input.remove_suffix(1);
    }
    return std::string(input);
}

inline std::string_view trim_view(std::string_view input)
{
    while (!input.empty() && std::isspace(static_cast<unsigned char>(input.front())))
    {
        input.remove_prefix(1);
    }
    while (!input.empty() && std::isspace(static_cast<unsigned char>(input.back())))
    {
        input.remove_suffix(1);
    }
    return input;
}

inline std::vector<std::string> split(std::string_view input, char delimiter, bool skipEmpty = true)
{
    std::vector<std::string> parts;
    std::string current;
    for (char ch : input)
    {
        if (ch == delimiter)
        {
            if (!current.empty() || !skipEmpty)
            {
                parts.push_back(current);
            }
            current.clear();
        }
        else
        {
            current.push_back(ch);
        }
    }
    if (!current.empty() || !skipEmpty)
    {
        parts.push_back(current);
    }
    return parts;
}

template <typename Range>
std::vector<std::string> split(Range &&range, char delimiter, bool skipEmpty = true)
{
    return split(std::string_view(range), delimiter, skipEmpty);
}

inline bool starts_with(std::string_view input, std::string_view prefix)
{
    return input.size() >= prefix.size() && input.substr(0, prefix.size()) == prefix;
}

inline bool ends_with(std::string_view input, std::string_view suffix)
{
    return input.size() >= suffix.size() && input.substr(input.size() - suffix.size()) == suffix;
}

template <typename T = int64_t, typename Range>
std::vector<T> to_numbers(const Range &range)
{
    std::vector<T> numbers;
    numbers.reserve(std::size(range));
    for (const auto &token : range)
    {
        T value{};
        const char *begin = token.data();
        const char *end = begin + token.size();
        const auto [ptr, ec] = std::from_chars(begin, end, value);
        if (ec != std::errc{} || ptr != end)
        {
            throw std::runtime_error("Failed to convert token to number");
        }
        numbers.push_back(value);
    }
    return numbers;
}

} // namespace common::str
