#pragma once

#include <string_view>
#include <vector>

#include "Grid.hpp"
#include "MathUtils.hpp"
#include "Search.hpp"
#include "StringUtils.hpp"
#include "BitsetUtils.hpp"

inline std::vector<std::string> splitString(std::string_view input, const char delimiter)
{
    return common::str::split(input, delimiter);
}

using Coordinate = common::grid::Coordinate;
using common::grid::NORTH;
using common::grid::SOUTH;
using common::grid::EAST;
using common::grid::WEST;
using common::grid::NORTH_EAST;
using common::grid::NORTH_WEST;
using common::grid::SOUTH_EAST;
using common::grid::SOUTH_WEST;

template <typename T>
inline bool inBoundary(std::vector<std::vector<T>> &map, Coordinate coord)
{
    return common::grid::inBounds(coord, map[0].size(), map.size());
}

inline bool inBoundary(Coordinate node, uint32_t map_width, uint32_t map_height)
{
    return common::grid::inBounds(node, map_width, map_height);
}