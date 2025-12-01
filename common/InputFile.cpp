#include "InputFile.hpp"

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

std::string InputFile::getText()
{
    std::string text;
    text.reserve(_lines.size() * 8); // heuristic reserve to reduce reallocations
    for (std::size_t i = 0; i < _lines.size(); ++i)
    {
        text += _lines[i];
        if (i + 1 < _lines.size())
        {
            text.push_back('\n');
        }
    }

    return text;
}