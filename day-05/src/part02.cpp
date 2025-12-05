/**
 * Day-5 - Part 02
 */
#include "include.hpp"
#include <algorithm>
#include <vector>
#include <cstring>

int64_t handlePart2(const InputFile &input) {
    const auto &lines = input.getLines();
    
    auto isEmpty = [](const auto& line){return !line.empty();};
    
    // Collect all ranges into a vector
    std::vector<std::pair<int64_t, int64_t>> rangeVec;
    for (const auto& line : lines)
    {
        if (line.empty()) break;
        
        const auto dashPos = line.find('-');
        const int64_t first = std::strtoll(line.c_str(), nullptr, 10);
        const int64_t second = std::strtoll(line.c_str() + dashPos + 1, nullptr, 10);
        rangeVec.emplace_back(first, second);
    }
    
    // Sort ranges by start position
    std::sort(rangeVec.begin(), rangeVec.end());
    
    // Merge overlapping/adjacent ranges and count total coverage
    int64_t totalCount = 0;
    if (!rangeVec.empty())
    {
        int64_t currentStart = rangeVec[0].first;
        int64_t currentEnd = rangeVec[0].second;
        
        for (size_t i = 1; i < rangeVec.size(); ++i)
        {
            const auto [start, end] = rangeVec[i];
            
            // Check if ranges overlap or are adjacent (end+1 == start)
            if (start <= currentEnd + 1)
            {
                // Merge: extend current range
                currentEnd = std::max(currentEnd, end);
            }
            else
            {
                // Gap found: count current range and start new one
                totalCount += (currentEnd - currentStart + 1);
                currentStart = start;
                currentEnd = end;
            }
        }
        
        // Add the last range
        totalCount += (currentEnd - currentStart + 1);
    }
    
    return totalCount;
}