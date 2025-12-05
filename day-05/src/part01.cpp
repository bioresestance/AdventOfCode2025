/**
 * Day-5 - Part 01
 */
#include "include.hpp"
#include <algorithm>
#include <vector>
#include <cstring>

int64_t handlePart1(const InputFile &input) {
    const auto &lines = input.getLines();
    
    // Parse ranges into a vector once
    std::vector<std::pair<int64_t, int64_t>> ranges;
    size_t i = 0;
    for (; i < lines.size() && !lines[i].empty(); ++i)
    {
        const auto dashPos = lines[i].find('-');
        const int64_t first = std::strtoll(lines[i].c_str(), nullptr, 10);
        const int64_t second = std::strtoll(lines[i].c_str() + dashPos + 1, nullptr, 10);
        ranges.emplace_back(first, second);
    }
    
    // Sort ranges by start position for potential early exit
    std::sort(ranges.begin(), ranges.end());
    
    // Parse and check IDs
    int64_t total = 0;
    for (++i; i < lines.size(); ++i)
    {
        if (lines[i].empty()) continue;
        
        const int64_t id = std::strtoll(lines[i].c_str(), nullptr, 10);
        
        // Binary search for first range that could contain id
        auto it = std::lower_bound(ranges.begin(), ranges.end(), id,
            [](const auto& range, int64_t value) { return range.second < value; });
        
        // Check if id falls within this range
        if (it != ranges.end() && id >= it->first && id <= it->second)
        {
            ++total;
        }
    }

    return total;
}