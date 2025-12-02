/**
 * Day-2 - Part 01
 */
#include "include.hpp"
#include <iostream>
#include <ranges>

using namespace std::ranges;

int64_t handlePart1(const std::vector<std::string> &inputLines) {

    // Should only have a single line 
    if( inputLines.size() != 1)
    {
        return 0;
    }

    auto numbers = splitString(inputLines[0], ',');
    std::cout << "Testing" << std::endl;

    uint64_t total = 0;

    for(const auto& num : numbers)
    {
        const auto parsed = splitString(num, '-');
        uint64_t min = std::stoull(parsed[0]);
        uint64_t max = std::stoull(parsed[1]);
        for(const auto curr : iota_view{min, max +1})
        {
            const auto strnum = std::to_string(curr);
            // Only check even length numbers.
            if((strnum.length() % 2) == 0)
            {
                const auto &half1 = strnum.substr(0, strnum.length()/2);
                const auto &half2 = strnum.substr(strnum.length()/2, strnum.length()/2);
                if( half1 == half2)
                {
                    total += curr;
                }
            }
        }
    }


    return total;
}