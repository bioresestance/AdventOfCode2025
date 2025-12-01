#pragma once

#include <cctype>
#include <charconv>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace day01::detail
{
    constexpr int kDialSize = 100;

    constexpr int wrap(int value)
    {
        value %= kDialSize;
        if (value < 0)
        {
            value += kDialSize;
        }
        return value;
    }

    inline std::string_view trim(std::string_view text)
    {
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())))
        {
            text.remove_prefix(1);
        }
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())))
        {
            text.remove_suffix(1);
        }
        return text;
    }

    template <typename Fn>
    void forEachInstruction(std::string_view input, Fn &&fn)
    {
        std::string_view remaining = input;
        while (!remaining.empty())
        {
            const auto newlineIdx = remaining.find('\n');
            std::string_view line = newlineIdx == std::string_view::npos ? remaining : remaining.substr(0, newlineIdx);
            remaining = newlineIdx == std::string_view::npos ? std::string_view{} : remaining.substr(newlineIdx + 1);

            line = trim(line);
            if (line.empty())
            {
                continue;
            }

            const char direction = line.front();
            if (direction != 'L' && direction != 'R')
            {
                throw std::runtime_error("Unexpected rotation direction");
            }

            line.remove_prefix(1);
            line = trim(line);

            int distance = 0;
            const char *const begin = line.data();
            const char *const end = begin + line.size();
            const auto [ptr, err] = std::from_chars(begin, end, distance);
            if (err != std::errc{} || ptr != end)
            {
                throw std::runtime_error("Invalid rotation distance in input");
            }

            fn(direction, distance);
        }
    }

    inline int applyRotation(int position, char direction, int distance)
    {
        const int shift = distance % kDialSize;
        if (direction == 'L')
        {
            return wrap(position - shift);
        }
        if (direction == 'R')
        {
            return wrap(position + shift);
        }
        throw std::runtime_error("Unexpected rotation direction");
    }

    inline std::uint64_t countZeroClicks(int position, char direction, int distance)
    {
        if (distance <= 0)
        {
            return 0;
        }

        const int normalized = wrap(position);
        int stepsToZero = 0;
        if (direction == 'R')
        {
            stepsToZero = (kDialSize - normalized) % kDialSize;
        }
        else if (direction == 'L')
        {
            stepsToZero = normalized % kDialSize;
        }
        else
        {
            throw std::runtime_error("Unexpected rotation direction");
        }

        if (stepsToZero == 0)
        {
            stepsToZero = kDialSize;
        }

        if (distance < stepsToZero)
        {
            return 0;
        }

        const std::uint64_t remainingAfterFirst = static_cast<std::uint64_t>(distance - stepsToZero);
        return 1 + remainingAfterFirst / kDialSize;
    }
}
