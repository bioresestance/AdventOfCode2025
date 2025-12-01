/**
 * Day-1 - Part 02
 */
#include "include.hpp"
#include "detail.hpp"

#include <cstdint>

namespace detail = day01::detail;

std::string handlePart2(const std::string_view input)
{
    int position = 50;
    std::uint64_t zeroHits = 0;

    detail::forEachInstruction(input, [&](char direction, int distance) {
        zeroHits += detail::countZeroClicks(position, direction, distance);
        position = detail::applyRotation(position, direction, distance);
    });

    return std::to_string(zeroHits);
}