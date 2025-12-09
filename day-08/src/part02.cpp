/**
 * Day-8 - Part 02
 */
#include "include.hpp"
#include <ranges>
#include <iostream>
#include <cmath>
#include <map>

using namespace std::ranges;

struct BoxPosition
{
    int64_t x;
    int64_t y;
    int64_t z;
};

using BoxPair = std::pair<uint32_t, uint32_t>;
using BoxPairAndDist = std::pair<BoxPair, uint64_t>;

static inline uint64_t distance(BoxPosition pos1, BoxPosition pos2)
{
    return static_cast<uint64_t>(std::pow(double(pos2.x - pos1.x), 2) + std::pow(double(pos2.y - pos1.y), 2) + std::pow(double(pos2.z - pos1.z), 2));
}

static std::vector<BoxPairAndDist> findAndSortDistances(const std::vector<BoxPosition> &pairs)
{
    std::vector<BoxPairAndDist> data;

    for (const auto [box1_idx, box1] : pairs | views::enumerate)
    {
        for (const auto [box2_idx, box2] : pairs | views::enumerate | views::drop(box1_idx + 1))
        {
            auto dist = ::distance(box1, box2);

            data.emplace_back(BoxPair{box1_idx, box2_idx}, dist);
        }
    }

    std::ranges::sort(data, [](auto box1, auto box2)
                      { return box1.second < box2.second; });
    return data;
}

static uint32_t findParent(std::vector<uint32_t> &parents, uint32_t point)
{
    if (parents[point] != point)
    {
        // Keep digging until we find the top parent of the set, and compress as we go.
        parents[point] = findParent(parents, parents[point]); // Path compression
    }

    return parents[point];
}

static uint32_t unionBoxes(std::vector<uint32_t> &parents, std::vector<uint32_t> &sizes, uint32_t numSets, uint32_t box1, uint32_t box2)
{
    auto box1Parent = findParent(parents, box1);
    auto box2Parent = findParent(parents, box2);

    if (box1Parent == box2Parent)
    {
        // Already in the same set, since they have the same root parent.
        return numSets;
    }
    if (sizes[box1Parent] < sizes[box2Parent])
    {
        std::swap(box1Parent, box2Parent);
    }
    parents[box2Parent] = box1Parent;
    sizes[box1Parent] += sizes[box2Parent];
    // Combined 2 sets to 1, so the number of sets decreases.
    return --numSets;
}

int64_t handlePart2(const InputFile &input)
{
    const auto &lines = input.getLines();

    std::vector<BoxPosition> boxes;
    std::map<uint32_t, uint32_t> circuits;

    // Parse all box positions.
    for (const auto &line : lines)
    {
        auto nums = common::str::to_vector_of_numbers(line, ',');
        boxes.emplace_back(nums[0], nums[1], nums[2]);
    }

    // Find all of the pairs and their distances between each other. Data is returned sorted, lowest to highest distance.
    auto data = findAndSortDistances(boxes);

    const auto numIters = (boxes.size() == 1000) ? 1000 : 10; // only do 10 iters for sample, else do the full 1000.

    std::vector<uint32_t> parents;
    std::vector<uint32_t> sizes;

    for (uint32_t i = 0; i < boxes.size(); i++)
    {
        parents.emplace_back(i); // Each point is a parent of itself.
        sizes.emplace_back(1);   // All points have a size of 1 to start.
    }

    uint32_t numSets = boxes.size();

    // Proccess the amount of iterations to get them into sets.
    for (uint32_t idx = 0; idx < data.size(); idx++)
    {
        numSets = unionBoxes(parents, sizes, numSets, data[idx].first.first, data[idx].first.second);

        if (numSets == 1)
        {
            return boxes[data[idx].first.first].x * boxes[data[idx].first.second].x;
        }
    }
    return 0; // Shouldnt get here.
}