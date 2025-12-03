#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <vector>

namespace common::grid
{
struct Coordinate
{
    int64_t x = 0;
    int64_t y = 0;

    Coordinate operator+(const Coordinate &other) const noexcept
    {
        return {x + other.x, y + other.y};
    }

    Coordinate operator-(const Coordinate &other) const noexcept
    {
        return {x - other.x, y - other.y};
    }

    Coordinate &operator+=(const Coordinate &other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Coordinate &operator-=(const Coordinate &other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    bool operator==(const Coordinate &other) const noexcept = default;
    bool operator!=(const Coordinate &other) const noexcept = default;

    friend std::ostream &operator<<(std::ostream &os, const Coordinate &coord)
    {
        os << '(' << coord.x << ", " << coord.y << ')';
        return os;
    }
};

inline constexpr Coordinate NORTH{0, -1};
inline constexpr Coordinate SOUTH{0, 1};
inline constexpr Coordinate EAST{1, 0};
inline constexpr Coordinate WEST{-1, 0};
inline constexpr Coordinate NORTH_EAST{1, -1};
inline constexpr Coordinate NORTH_WEST{-1, -1};
inline constexpr Coordinate SOUTH_EAST{1, 1};
inline constexpr Coordinate SOUTH_WEST{-1, 1};

inline bool inBounds(Coordinate coord, std::size_t width, std::size_t height)
{
    return coord.x >= 0 && coord.y >= 0 && static_cast<std::size_t>(coord.x) < width &&
           static_cast<std::size_t>(coord.y) < height;
}

inline std::vector<Coordinate> orthogonalNeighbors(Coordinate coord)
{
    return {coord + NORTH, coord + SOUTH, coord + EAST, coord + WEST};
}

inline std::vector<Coordinate> diagonalNeighbors(Coordinate coord)
{
    return {coord + NORTH_EAST, coord + NORTH_WEST, coord + SOUTH_EAST, coord + SOUTH_WEST};
}

inline std::vector<Coordinate> allNeighbors(Coordinate coord)
{
    auto neighbors = orthogonalNeighbors(coord);
    const auto diagonals = diagonalNeighbors(coord);
    neighbors.insert(neighbors.end(), diagonals.begin(), diagonals.end());
    return neighbors;
}

template <typename T>
class Grid
{
public:
    Grid() = default;
    Grid(std::size_t width, std::size_t height, T value = {})
        : m_width(width), m_height(height), m_cells(width * height, value)
    {
    }

    T &operator()(std::size_t x, std::size_t y)
    {
        return m_cells[y * m_width + x];
    }

    const T &operator()(std::size_t x, std::size_t y) const
    {
        return m_cells[y * m_width + x];
    }

    std::size_t width() const noexcept { return m_width; }
    std::size_t height() const noexcept { return m_height; }

    bool contains(Coordinate coord) const noexcept
    {
        return inBounds(coord, m_width, m_height);
    }

private:
    std::size_t m_width = 0;
    std::size_t m_height = 0;
    std::vector<T> m_cells;
};

} // namespace common::grid

namespace std
{
template <>
struct hash<common::grid::Coordinate>
{
    std::size_t operator()(const common::grid::Coordinate &coord) const noexcept
    {
        return std::hash<int64_t>()(coord.x) ^ (std::hash<int64_t>()(coord.y) << 1);
    }
};
}
