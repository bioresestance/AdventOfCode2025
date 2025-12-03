#include "InputFile.hpp"

#include <charconv>
#include <stdexcept>

InputFile::InputFile(std::string filename) : _filename(filename)
{
    // Open the file
    _file.open(filename);
    if (!_file.is_open())
    {
        std::cout << "Could not open file: " << filename << std::endl;
        return;
    }

    // Rad in all the lines and store them in the vector
    std::string line;
    while (std::getline(_file, line))
    {
        _lines.push_back(line);
    }

    // Close the file
    _file.close();
}

std::vector<std::string> &InputFile::getLines()
{
    return _lines;
}

const std::vector<std::string> &InputFile::getLines() const
{
    return _lines;
}

const std::string &InputFile::getText() const
{
    if (!_text.has_value())
    {
        _text.emplace();
        _text->reserve(_lines.size() * 8);
        for (std::size_t i = 0; i < _lines.size(); ++i)
        {
            *_text += _lines[i];
            if (i + 1 < _lines.size())
            {
                _text->push_back('\n');
            }
        }
    }
    return *_text;
}

const std::vector<int64_t> &InputFile::asIntegers() const
{
    if (!_integers.has_value())
    {
        _integers.emplace();
        _integers->reserve(_lines.size());
        for (const auto &line : _lines)
        {
            if (line.empty())
            {
                continue;
            }

            int64_t value = 0;
            const char *const begin = line.data();
            const char *const end = begin + line.size();
            const auto [ptr, ec] = std::from_chars(begin, end, value);
            if (ec != std::errc{} || ptr != end)
            {
                throw std::runtime_error("Failed to parse integer from input line: " + line);
            }
            _integers->push_back(value);
        }
    }
    return *_integers;
}

const std::vector<std::vector<char>> &InputFile::asGrid() const
{
    if (!_grid.has_value())
    {
        _grid.emplace();
        _grid->reserve(_lines.size());
        for (const auto &line : _lines)
        {
            _grid->push_back({line.begin(), line.end()});
        }
    }
    return *_grid;
}