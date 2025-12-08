/**
 * Day-7 - Part 02
 */
#include "include.hpp"
#include <ranges>

using namespace std::ranges;

int64_t handlePart2(const InputFile &input)
{
    const auto &grid = input.asGrid();

    // Track number of timelines with an active beam at each cell
    // Instead of recursing, we propagate beam counts row by row
    common::grid::Grid<uint64_t> beamCount(grid.width(), grid.height(), 0);

    // Find starting position (S) and initialize with 1 beam
    for (const auto coord : grid.coordinates())
    {
        if (grid[coord] == 'S')
        {
            beamCount[coord] = 1;
            break;
        }
    }

    // Process row by row (top to bottom)
    for (std::size_t y = 1; y < grid.height(); ++y)
    {
        for (std::size_t x = 0; x < grid.width(); ++x)
        {
            Coordinate cell{static_cast<int64_t>(x), static_cast<int64_t>(y)};
            Coordinate aboveCell = cell + NORTH;

            uint64_t beamsFromAbove = beamCount[aboveCell];
            if (beamsFromAbove == 0)
            {
                continue;
            }

            char cellChar = grid[cell];

            if (cellChar == '.')
            {
                // Beam continues straight down
                beamCount[cell] += beamsFromAbove;
            }
            else if (cellChar == '^')
            {
                // Splitter: beam splits left and right, each path creates its own timeline
                Coordinate leftCell = cell + WEST;
                Coordinate rightCell = cell + EAST;

                if (grid.contains(leftCell) && grid[leftCell] != '^')
                {
                    beamCount[leftCell] += beamsFromAbove;
                }
                if (grid.contains(rightCell) && grid[rightCell] != '^')
                {
                    beamCount[rightCell] += beamsFromAbove;
                }
            }
        }
    }

    // Count total timelines: sum of all beams in the bottom row
    uint64_t totalTimelines = 0;
    std::size_t lastRow = grid.height() - 1;
    for (std::size_t x = 0; x < grid.width(); ++x)
    {
        totalTimelines += beamCount[Coordinate{static_cast<int64_t>(x), static_cast<int64_t>(lastRow)}];
    }

    return totalTimelines;
}