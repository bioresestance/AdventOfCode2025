/**
 * Day-7 - Part 01
 */
#include "include.hpp"
#include <ranges>

using namespace std::ranges;

int64_t handlePart1(const InputFile &input)
{
    auto grid = input.asGrid();

    uint32_t splits = 0;

    // Go through all cells, skiping the first row
    for (const auto cell : grid.coordinates() | views::drop(grid.width()))
    {

        auto aboveCell = cell - Coordinate{0, 1};

        if (grid[aboveCell] == '|' or grid[aboveCell] == 'S')
        {
            if (grid[cell] == '.')
            {
                grid[cell] = '|';
            }

            if (grid[cell] == '^')
            {
                const auto leftCell = cell - Coordinate{1, 0};
                const auto rightCell = cell + Coordinate{1, 0};
                splits++;

                if (grid.contains(leftCell) and grid[leftCell] != '^')
                {
                    grid[leftCell] = '|';
                }
                if (grid.contains(rightCell) and grid[rightCell] != '^')
                {
                    grid[rightCell] = '|';
                }
            }
        }
    }

    return splits;
}