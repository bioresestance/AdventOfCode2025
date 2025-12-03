/**
 * Day-2 - Part 02
 */
#include "include.hpp"
#include <iostream>
#include <ranges>

using namespace std::ranges;

int64_t handlePart2(const InputFile &input) {
    const auto &inputLines = input.getLines();
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


            // We want to split up the string into substring on varying length, up to half the length.
            for(const auto len : iota_view{1, (int)((strnum.length()/2)+1)})
            {
                // Check if the string is divisable evenly, then it wouldnt work
                if(strnum.length() % len != 0)
                {
                    continue;
                }

                const auto & initialstr = strnum.substr(0, len);
                bool checked = true;
                
                // Now go through all substrings past the first to see if they are all the same.
                for(uint32_t i = 1; i <= (strnum.length() / len) - 1; i++)
                {
                    const auto & newstr = strnum.substr(len*i, len);
                    if(initialstr != newstr)
                    {
                        checked = false;
                        break;
                    }
                }
                // If all substrings were the same, then it matches the pattern and the number is an invalid ID.
                if(checked)
                {
                    total += curr;
                    // No point checking the rest of the permutations for the current number.
                    break;
                }

            }
        }
    }


    return total;
}