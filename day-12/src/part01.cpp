/**
 * Day-12 - Part 01
 */
#include "include.hpp"
#include <ranges>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>

using namespace std::ranges;

// Parse a shape definition and return the number of '#' cells
int parseShapeSize(const std::vector<std::string> &lines, size_t &lineIdx)
{
    int size = 0;
    while (lineIdx < lines.size() && !lines[lineIdx].empty() && lines[lineIdx].find(':') == std::string::npos)
    {
        for (char c : lines[lineIdx])
        {
            if (c == '#')
                size++;
        }
        lineIdx++;
    }
    return size;
}

int64_t handlePart1(const InputFile &input)
{
    const auto &lines = input.getLines();

    // Parse shape sizes
    std::vector<int> shapeSizes;
    size_t lineIdx = 0;

    // Parse all shapes
    while (lineIdx < lines.size())
    {
        const auto &line = lines[lineIdx];

        // Check if this line starts a shape definition (e.g., "0:")
        if (line.find(':') != std::string::npos && line.find('x') == std::string::npos)
        {
            lineIdx++; // Skip the "N:" line
            int size = parseShapeSize(lines, lineIdx);
            shapeSizes.push_back(size);
        }
        else if (line.find('x') != std::string::npos)
        {
            // We've reached the regions section
            break;
        }
        else
        {
            lineIdx++;
        }
    }

    // Now parse regions and count how many can fit
    int64_t count = 0;

    while (lineIdx < lines.size())
    {
        const auto &line = lines[lineIdx];
        if (line.empty())
        {
            lineIdx++;
            continue;
        }

        // Parse region: "WxH: c0 c1 c2 c3 c4 c5"
        size_t xPos = line.find('x');
        size_t colonPos = line.find(':');

        if (xPos != std::string::npos && colonPos != std::string::npos)
        {
            int width = std::stoi(line.substr(0, xPos));
            int height = std::stoi(line.substr(xPos + 1, colonPos - xPos - 1));
            int64_t area = static_cast<int64_t>(width) * height;

            // Parse counts
            std::vector<int> counts;
            std::istringstream iss(line.substr(colonPos + 1));
            int c;
            while (iss >> c)
            {
                counts.push_back(c);
            }

            // Calculate total cells needed and total presents
            int64_t cellsNeeded = 0;
            int64_t totalPresents = 0;
            for (size_t i = 0; i < counts.size() && i < shapeSizes.size(); i++)
            {
                cellsNeeded += static_cast<int64_t>(counts[i]) * shapeSizes[i];
                totalPresents += counts[i];
            }

            // Calculate slack
            int64_t slack = area - cellsNeeded;
            int64_t threshold = 2 * (totalPresents - 1);

            // Region fits if slack >= threshold
            if (slack >= threshold)
            {
                count++;
            }
        }
        lineIdx++;
    }

    return count;
}