/**
 * Day-9 - Part 02
 */
#include "include.hpp"
#include <ranges>
#include <iostream>
#include <unordered_set>
#include <algorithm>

using namespace std::ranges;

// Store polygon edges for efficient rectangle checking
struct Edge
{
    Coordinate start;
    Coordinate end;
    bool isVertical;
};

// Check if a rectangle is fully inside the polygon using edge intersection
// A rectangle is valid if:
// 1. All 4 corners are inside the polygon
// 2. No polygon edge crosses through the interior of the rectangle
bool isPointInPolygon(const std::vector<Coordinate> &polygon, Coordinate point)
{
    // Ray casting algorithm
    int crossings = 0;
    size_t n = polygon.size();

    for (size_t i = 0; i < n; ++i)
    {
        const auto &p1 = polygon[i];
        const auto &p2 = polygon[(i + 1) % n];

        // Check if ray from point going right crosses this edge
        if ((p1.y <= point.y && p2.y > point.y) || (p2.y <= point.y && p1.y > point.y))
        {
            // Compute x coordinate of intersection
            double intersectX = (double)(p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x;
            if (point.x < intersectX)
            {
                crossings++;
            }
        }
    }

    return (crossings % 2) == 1;
}

// Check if a vertical edge intersects the interior of a rectangle (not just touches boundary)
bool verticalEdgeCrossesRect(int64_t edgeX, int64_t edgeY1, int64_t edgeY2,
                             int64_t rectMinX, int64_t rectMaxX, int64_t rectMinY, int64_t rectMaxY)
{
    // Edge must be strictly inside rectangle's x range
    if (edgeX <= rectMinX || edgeX >= rectMaxX)
        return false;

    auto [eMinY, eMaxY] = std::minmax(edgeY1, edgeY2);

    // Check if edge's y range overlaps with rectangle's interior y range
    return eMinY < rectMaxY && eMaxY > rectMinY;
}

// Check if a horizontal edge intersects the interior of a rectangle
bool horizontalEdgeCrossesRect(int64_t edgeY, int64_t edgeX1, int64_t edgeX2,
                               int64_t rectMinX, int64_t rectMaxX, int64_t rectMinY, int64_t rectMaxY)
{
    // Edge must be strictly inside rectangle's y range
    if (edgeY <= rectMinY || edgeY >= rectMaxY)
        return false;

    auto [eMinX, eMaxX] = std::minmax(edgeX1, edgeX2);

    // Check if edge's x range overlaps with rectangle's interior x range
    return eMinX < rectMaxX && eMaxX > rectMinX;
}

int64_t handlePart2(const InputFile &input)
{
    const auto &lines = input.getLines();

    std::vector<Coordinate> cornerTiles;

    // Get the red tiles (corners of polygon)
    for (const auto &line : lines)
    {
        auto nums = common::str::to_vector_of_numbers(line, ',');
        cornerTiles.emplace_back(nums[0], nums[1]);
    }

    // Build edge list
    std::vector<Edge> edges;
    for (std::size_t i = 0; i < cornerTiles.size(); ++i)
    {
        const auto &t1 = cornerTiles[i];
        const auto &t2 = cornerTiles[(i + 1) % cornerTiles.size()];
        edges.push_back({t1, t2, t1.x == t2.x});
    }

    // Create a set of red tiles for fast lookup
    std::unordered_set<Coordinate> redTileSet(cornerTiles.begin(), cornerTiles.end());

    uint64_t maxArea = 0;

    for (std::size_t i = 0; i < cornerTiles.size(); ++i)
    {
        const auto &tile1 = cornerTiles[i];

        for (std::size_t j = i + 1; j < cornerTiles.size(); ++j)
        {
            const auto &tile2 = cornerTiles[j];

            // Skip if same row or column (degenerate rectangle)
            if (tile1.x == tile2.x || tile1.y == tile2.y)
                continue;

            auto [minX, maxX] = std::minmax(tile1.x, tile2.x);
            auto [minY, maxY] = std::minmax(tile1.y, tile2.y);

            // Check all 4 corners are inside or on the polygon
            Coordinate corners[4] = {
                {minX, minY}, {maxX, minY}, {minX, maxY}, {maxX, maxY}};

            bool allCornersValid = true;
            for (const auto &corner : corners)
            {
                // Corner is valid if it's ANY red tile or inside the polygon
                bool isRedTile = redTileSet.contains(corner);
                if (!isRedTile && !isPointInPolygon(cornerTiles, corner))
                {
                    allCornersValid = false;
                    break;
                }
            }

            if (!allCornersValid)
                continue;

            // Check no polygon edge crosses through the rectangle interior
            bool edgeCrossesInterior = false;
            for (const auto &edge : edges)
            {
                if (edge.isVertical)
                {
                    if (verticalEdgeCrossesRect(edge.start.x, edge.start.y, edge.end.y,
                                                minX, maxX, minY, maxY))
                    {
                        edgeCrossesInterior = true;
                        break;
                    }
                }
                else
                {
                    if (horizontalEdgeCrossesRect(edge.start.y, edge.start.x, edge.end.x,
                                                  minX, maxX, minY, maxY))
                    {
                        edgeCrossesInterior = true;
                        break;
                    }
                }
            }

            if (edgeCrossesInterior)
                continue;

            auto width = maxX - minX + 1;
            auto height = maxY - minY + 1;
            auto area = static_cast<uint64_t>(width * height);
            maxArea = std::max(maxArea, area);
        }
    }

    return maxArea;
}