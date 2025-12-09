/**
 * Day-9 - Part 01
 */
#include "include.hpp"
#include <ranges>
#include <iostream>

using namespace std::ranges;

int64_t handlePart1(const InputFile &input)
{
    const auto &lines = input.getLines();

    std::vector<Coordinate> tiles;

    for (const auto &line : lines)
    {
        auto nums = common::str::to_vector_of_numbers(line, ',');
        tiles.emplace_back(nums[0], nums[1]);
    }

    uint64_t maxArea = 0;
    for (const auto [tile1Idx, tile1] : tiles | views::enumerate)
    {
        for (const auto tile2 : tiles | views::drop(tile1Idx + 1))
        {
            auto area = static_cast<uint64_t>(std::abs(tile1.x - tile2.x + 1) * std::abs(tile1.y - tile2.y + 1));
            maxArea = std::max(area, maxArea);
        }
    }

    return maxArea;
}