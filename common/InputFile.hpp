#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

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
    const std::vector<std::vector<char>> &asGrid() const;

    iterator begin() const noexcept { return _lines.begin(); }
    iterator end() const noexcept { return _lines.end(); }

private:
    /// @brief Filename to read
    std::string _filename;
    /// @brief Vector of lines from the file
    std::vector<std::string> _lines;
    /// @brief File stream
    std::ifstream _file;
    /// @brief Cached blob of the entire file
    mutable std::optional<std::string> _text;
    /// @brief Cached integer representation of every non-empty line
    mutable std::optional<std::vector<int64_t>> _integers;
    /// @brief Cached grid representation
    mutable std::optional<std::vector<std::vector<char>>> _grid;
};