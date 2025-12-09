/**
 * Day-8 - Part 01
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

inline uint64_t distance(BoxPosition pos1, BoxPosition pos2)
{
    return static_cast<uint64_t>(std::pow(double(pos2.x - pos1.x), 2) + std::pow(double(pos2.y - pos1.y), 2) + std::pow(double(pos2.z - pos1.z), 2));
}

std::vector<BoxPairAndDist> findAndSortDistances(const std::vector<BoxPosition> &pairs)
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

uint32_t findParent(std::vector<uint32_t> &parents, uint32_t point)
{
    if (parents[point] != point)
    {
        // Keep digging until we find the top parent of the set, and compress as we go.
        parents[point] = findParent(parents, parents[point]); // Path compression
    }

    return parents[point];
}

void unionBoxes(std::vector<uint32_t> &parents, std::vector<uint32_t> &sizes, uint32_t box1, uint32_t box2)
{
    auto box1Parent = findParent(parents, box1);
    auto box2Parent = findParent(parents, box2);

    if (box1Parent == box2Parent)
    {
        // Already in the same set, since they have the same root parent.
        return;
    }
    if (sizes[box1Parent] < sizes[box2Parent])
    {
        std::swap(box1Parent, box2Parent);
    }
    parents[box2Parent] = box1Parent;
    sizes[box1Parent] += sizes[box2Parent];
}

int64_t handlePart1(const InputFile &input)
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

    // Proccess the amount of iterations to get them into sets.
    for (uint32_t idx = 0; idx < numIters; idx++)
    {
        unionBoxes(parents, sizes, data[idx].first.first, data[idx].first.second);
    }

    for (uint32_t i = 0; i < boxes.size(); i++)
    {
        auto rootParent = findParent(parents, i);
        circuits[rootParent]++;
    }

    std::vector<std::pair<uint32_t, uint32_t>> vec;
    for (const auto &p : circuits)
    {
        vec.push_back(p);
    }
    std::ranges::sort(vec, greater{}, &std::pair<uint32_t, uint32_t>::second);

    return vec[0].second * vec[1].second * vec[2].second;
}