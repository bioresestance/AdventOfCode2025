/**
 * Day-11 - Part 02
 */
#include "include.hpp"
#include <ranges>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std::ranges;

using DevMap = std::unordered_map<std::string, std::vector<std::string>>;
static DevMap devices;
static const std::string START = "svr";
static const std::string OUT = "out";

// Memoization: map from (node, foundFft, foundDac) -> count of valid paths
static std::unordered_map<std::string, uint64_t> memo;

static std::string makeKey(const std::string &node, bool foundFft, bool foundDac)
{
    return node + "_" + (foundFft ? "1" : "0") + "_" + (foundDac ? "1" : "0");
}

static uint64_t findExit(const std::string &currDevice, bool foundFft, bool foundDac,
                         std::unordered_set<std::string> &visited)
{
    // Check if we've already computed this state
    auto key = makeKey(currDevice, foundFft, foundDac);
    if (memo.count(key))
    {
        return memo[key];
    }

    // Detect cycles
    if (visited.count(currDevice))
    {
        return 0;
    }

    // Update flags if we've found fft or dac
    if (currDevice == "dac")
    {
        foundDac = true;
    }
    if (currDevice == "fft")
    {
        foundFft = true;
    }

    auto &next = devices[currDevice];

    // Base case: reached OUT
    if (next[0] == OUT)
    {
        uint64_t result = (foundDac && foundFft) ? 1 : 0;
        memo[key] = result;
        return result;
    }

    // Mark as visited for this path
    visited.insert(currDevice);

    uint64_t totalPaths = 0;
    for (const auto &link : next)
    {
        if (link == START)
        {
            continue;
        }

        totalPaths += findExit(link, foundFft, foundDac, visited);
    }

    // Unmark for backtracking
    visited.erase(currDevice);

    memo[key] = totalPaths;
    return totalPaths;
}

int64_t handlePart2(const InputFile &input)
{
    // Reset globals in case samples and real input run in the same process
    devices.clear();
    memo.clear();

    const auto &lines = input.getLines();

    // Parse out the lines into a map.
    for (const auto &line : lines)
    {
        const auto parts = common::str::split(line, ':');
        const auto &key = parts[0];
        const auto outputs = common::str::split(parts[1], ' ');
        devices[key] = std::move(outputs);
    }

    std::unordered_set<std::string> visited;
    return findExit(START, false, false, visited);
}