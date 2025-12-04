/**
 * Day-4 - Part 02
 */
#include "include.hpp"

int64_t handlePart2(const InputFile &input)
{
    auto map = input.asGrid();
    int64_t totalRemoved = 0;

    bool hadChanges = false;
    do
    {
        hadChanges = false;

        for (const auto coord : map.coordinates())
        {
            if (map[coord] != '@')
            {
                continue;
            }

            const auto neighbors = map.allNeighbors(coord);
            
            int activeCount = 0;
            for (const auto &cell : neighbors)
            {
                if (map[cell] == '@')
                {
                    ++activeCount;
                }
            }

            if (activeCount < 4)
            {
                map[coord] = '.';
                hadChanges = true;
                ++totalRemoved;
            }
        }

    } while (hadChanges);

    return totalRemoved;
}