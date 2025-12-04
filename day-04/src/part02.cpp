/**
 * Day-4 - Part 02
 */
#include "include.hpp"

int64_t handlePart2(const InputFile &input)
{
    auto map = input.asGrid();
    uint32_t total = 0;

    uint32_t change = 0;
    do
    {
        change = 0;
        for (const auto &coord : map.coordinates())
        {

            if (map[coord] != '@')
            {
                continue;
            }

            const auto &neighbours = map.allNeighbors(coord);

            uint32_t count = 0;
            for (const auto &cell : neighbours)
            {
                if (map[cell] == '@')
                {
                    count++;
                }
            }
            if (count < 4)
            {
                map[coord] = '.';
                change++;
                total++;
            }
        }
    } while (change > 0);
    return total;
}