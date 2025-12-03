/**
 * Day-1 - Part 01
 */
#include "include.hpp"
#include "detail.hpp"

#include <cstdint>

namespace detail = day01::detail;

int64_t handlePart1(const InputFile &input)
{
    int position = 50;
    std::uint64_t zeroHits = 0;

    detail::forEachInstruction(input.getLines(), [&](char direction, int distance) {
        position = detail::applyRotation(position, direction, distance);
        if (position == 0)
        {
            ++zeroHits;
        }
    });

    return static_cast<int64_t>(zeroHits);
}