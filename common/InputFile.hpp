#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Grid.hpp"

class InputFile
{
public:
    using iterator = std::vector<std::string>::const_iterator;
    using const_iterator = std::vector<std::string>::const_iterator;

    /**
     * @brief Takes in a filename and reads the file into a vector of lines
     *
     * @param filename Filename to read
     */
    InputFile(std::string filename = "./input.txt");

    /**
     * @brief Construct an InputFile directly from in-memory lines.
     */
    InputFile(std::string filename, std::vector<std::string> lines);
    ~InputFile() = default;

    /**
     * @brief Returns a vector of lines from the file
     *
     * @return std::vector<std::string> Vector of lines from the file.
     */
    std::vector<std::string> &getLines();
    const std::vector<std::string> &getLines() const;

    /**
     * @brief Returns the text from the file as a single string
     *
     * @return std::string Text from the file as a single string
     */
    const std::string &getText() const;

    /**
     * @brief Returns parsed integers (cache is reused across calls)
     */
    const std::vector<int64_t> &asIntegers() const;

    /**
     * @brief Returns the input as a character grid (cache is reused across calls)
     */
    const common::grid::Grid<char> &asGrid() const;

    iterator begin() const noexcept { return _lines.begin(); }
    iterator end() const noexcept { return _lines.end(); }

    static InputFile fromLines(std::vector<std::string> lines,
                               std::string filename = "<memory>");

private:
    /// @brief Filename to read
    std::string _filename;
    /// @brief Vector of lines from the file
    std::vector<std::string> _lines;
    /// @brief Cached blob of the entire file
    mutable std::optional<std::string> _text;
    /// @brief Cached integer representation of every non-empty line
    mutable std::optional<std::vector<int64_t>> _integers;
    /// @brief Cached grid representation
    mutable std::optional<common::grid::Grid<char>> _grid;
};