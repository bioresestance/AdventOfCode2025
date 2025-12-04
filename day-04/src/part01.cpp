/**
 * Day-4 - Part 01
 */
#include "include.hpp"

int64_t handlePart1(const InputFile &input) {
    const auto& map = input.asGrid();
    uint32_t total = 0;

    for(const auto& coord : map.coordinates())
    {
        if(map[coord] != '@')
        {
            continue;
        }

        const auto& neighbours = map.allNeighbors(coord);


        uint32_t count = 0;
        for(const auto& cell : neighbours)
        {
            if(map[cell] == '@' )
            {
                count++;
            }
        }
        if(count < 4)
        {
            total++;
        }
        
    }
    return total;
}