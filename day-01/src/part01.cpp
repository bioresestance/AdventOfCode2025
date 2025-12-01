/**
 * Day-1 - Part 01
 */
#include "include.hpp"
#include "detail.hpp"

#include <cstdint>

namespace detail = day01::detail;

std::string handlePart1(const std::string_view input)
{
    int position = 50;
    std::uint64_t zeroHits = 0;

    detail::forEachInstruction(input, [&](char direction, int distance) {
        position = detail::applyRotation(position, direction, distance);
        if (position == 0)
        {
            ++zeroHits;
        }
    });

    return std::to_string(zeroHits);
}