#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <array>
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

template <typename T = int64_t>
std::vector<T> to_vector_of_numbers(std::string_view input, std::optional<char> delimiter = std::nullopt)
{
    if (delimiter.has_value())
    {
        auto tokens = split(input, *delimiter, true);
        std::vector<std::string> cleaned;
        cleaned.reserve(tokens.size());
        for (auto &token : tokens)
        {
            auto trimmed = trim_copy(token);
            if (trimmed.empty())
            {
                continue;
            }
            cleaned.push_back(std::move(trimmed));
        }
        return to_numbers<T>(cleaned);
    }

    std::vector<T> numbers;
    numbers.reserve(input.size());
    for (char ch : input)
    {
        if (std::isspace(static_cast<unsigned char>(ch)))
        {
            continue;
        }

        char buffer[1] = {ch};
        T value{};
        const auto [ptr, ec] = std::from_chars(std::begin(buffer), std::end(buffer), value);
        if (ec != std::errc{} || ptr != std::end(buffer))
        {
            throw std::runtime_error("Failed to convert character to number");
        }
        numbers.push_back(value);
    }
    return numbers;
}

inline std::string remove_chars(std::string_view input, std::string_view chars)
{
    if (chars.empty())
    {
        return std::string(input);
    }

    std::array<bool, 256> lookup{};
    for (unsigned char c : chars)
    {
        lookup[c] = true;
    }

    std::string result;
    result.reserve(input.size());
    for (unsigned char c : input)
    {
        if (!lookup[c])
        {
            result.push_back(static_cast<char>(c));
        }
    }
    return result;
}

inline std::string &remove_chars_inplace(std::string &input, std::string_view chars)
{
    if (chars.empty())
    {
        return input;
    }

    std::array<bool, 256> lookup{};
    for (unsigned char c : chars)
    {
        lookup[c] = true;
    }

    input.erase(std::remove_if(input.begin(), input.end(), [&](unsigned char c) { return lookup[c]; }), input.end());
    return input;
}

inline std::string remove_char(std::string_view input, char ch)
{
    return remove_chars(input, std::string_view(&ch, 1));
}

inline std::string &remove_char_inplace(std::string &input, char ch)
{
    return remove_chars_inplace(input, std::string_view(&ch, 1));
}

} // namespace common::str