#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <ranges>
#include <stdexcept>
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

    template <std::size_t N>
    inline std::vector<Coordinate> filterNeighbors(Coordinate coord,
                                                   const std::array<Coordinate, N> &offsets,
                                                   std::size_t width,
                                                   std::size_t height)
    {
        std::vector<Coordinate> neighbors;
        neighbors.reserve(N);
        for (const auto &offset : offsets)
        {
            const auto candidate = coord + offset;
            if (inBounds(candidate, width, height))
            {
                neighbors.push_back(candidate);
            }
        }
        return neighbors;
    }

    inline std::vector<Coordinate> orthogonalNeighbors(Coordinate coord,
                                                       std::size_t width,
                                                       std::size_t height)
    {
        static constexpr std::array<Coordinate, 4> kOffsets{NORTH, SOUTH, EAST, WEST};
        return filterNeighbors(coord, kOffsets, width, height);
    }

    inline std::vector<Coordinate> diagonalNeighbors(Coordinate coord,
                                                     std::size_t width,
                                                     std::size_t height)
    {
        static constexpr std::array<Coordinate, 4> kOffsets{NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};
        return filterNeighbors(coord, kOffsets, width, height);
    }

    inline std::vector<Coordinate> allNeighbors(Coordinate coord,
                                                std::size_t width,
                                                std::size_t height)
    {
        auto neighbors = orthogonalNeighbors(coord, width, height);
        auto diagonals = diagonalNeighbors(coord, width, height);
        neighbors.insert(neighbors.end(), diagonals.begin(), diagonals.end());
        return neighbors;
    }

    struct ToCoordinate
    {
        std::size_t width = 0;

        constexpr Coordinate operator()(std::size_t index) const noexcept
        {
            if (width == 0)
            {
                return {0, 0};
            }
            const auto x = index % width;
            const auto y = index / width;
            return {static_cast<int64_t>(x), static_cast<int64_t>(y)};
        }
    };

    class CoordinateRange : public std::ranges::view_interface<CoordinateRange>, public std::ranges::view_base
    {
    public:
        CoordinateRange() = default;

        CoordinateRange(std::size_t width, std::size_t height)
            : m_width(width), m_height(height),
              m_view(std::ranges::iota_view<std::size_t, std::size_t>{0, width * height}, ToCoordinate{width})
        {
        }

        constexpr auto begin() const noexcept { return m_view.begin(); }
        constexpr auto end() const noexcept { return m_view.end(); }
        constexpr std::size_t size() const noexcept { return m_view.size(); }

    private:
        using Iota = std::ranges::iota_view<std::size_t, std::size_t>;
        using View = std::ranges::transform_view<Iota, ToCoordinate>;

        std::size_t m_width = 0;
        std::size_t m_height = 0;
        View m_view{};
    };

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

        T &operator[](Coordinate coord)
        {
            ensureContains(coord);
            return (*this)(static_cast<std::size_t>(coord.x), static_cast<std::size_t>(coord.y));
        }

        const T &operator[](Coordinate coord) const
        {
            ensureContains(coord);
            return (*this)(static_cast<std::size_t>(coord.x), static_cast<std::size_t>(coord.y));
        }

        std::size_t width() const noexcept { return m_width; }
        std::size_t height() const noexcept { return m_height; }
        std::size_t size() const noexcept { return m_cells.size(); }

        bool contains(Coordinate coord) const noexcept
        {
            return inBounds(coord, m_width, m_height);
        }

        CoordinateRange coordinates() const noexcept { return CoordinateRange(m_width, m_height); }

        auto begin() noexcept { return m_cells.begin(); }
        auto end() noexcept { return m_cells.end(); }
        auto begin() const noexcept { return m_cells.begin(); }
        auto end() const noexcept { return m_cells.end(); }
        auto cbegin() const noexcept { return m_cells.cbegin(); }
        auto cend() const noexcept { return m_cells.cend(); }
        auto rbegin() noexcept { return m_cells.rbegin(); }
        auto rend() noexcept { return m_cells.rend(); }
        auto rbegin() const noexcept { return m_cells.rbegin(); }
        auto rend() const noexcept { return m_cells.rend(); }
        auto crbegin() const noexcept { return m_cells.crbegin(); }
        auto crend() const noexcept { return m_cells.crend(); }

        std::vector<Coordinate> orthogonalNeighbors(Coordinate coord) const
        {
            return common::grid::orthogonalNeighbors(coord, m_width, m_height);
        }

        std::vector<Coordinate> diagonalNeighbors(Coordinate coord) const
        {
            return common::grid::diagonalNeighbors(coord, m_width, m_height);
        }

        std::vector<Coordinate> allNeighbors(Coordinate coord) const
        {
            return common::grid::allNeighbors(coord, m_width, m_height);
        }

    private:
        void ensureContains(Coordinate coord) const
        {
            if (!contains(coord))
            {
                throw std::out_of_range("Grid coordinate out of bounds");
            }
        }

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
