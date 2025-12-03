/**
 * Day-1 - Part 02
 */
#include "include.hpp"
#include "detail.hpp"

#include <cstdint>

namespace detail = day01::detail;

int64_t handlePart2(const InputFile &input)
{
    int position = 50;
    std::uint64_t zeroHits = 0;

    detail::forEachInstruction(input.getLines(), [&](char direction, int distance) {
        zeroHits += detail::countZeroClicks(position, direction, distance);
        position = detail::applyRotation(position, direction, distance);
    });

    return static_cast<int64_t>(zeroHits);
}