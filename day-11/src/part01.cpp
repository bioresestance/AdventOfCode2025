/**
 * Day-11 - Part 01
 */
#include "include.hpp"
#include <ranges>
#include <iostream>
#include <unordered_map>

using namespace std::ranges;

using DevMap = std::unordered_map<std::string, std::vector<std::string>>;
static DevMap devices;
static uint64_t total = 0;
static const std::string YOU = "you";
static const std::string OUT = "out";

static void findExit(const std::string &currDevice)
{

    auto &next = devices[currDevice];

    if (next[0] != OUT)
    {
        for (const auto &link : next)
        {
            if (link == YOU)
            {
                continue;
            }

            findExit(link);
        }
    }
    else
    {
        // Found OUT, so add to count and return.
        total++;
    }
}

int64_t handlePart1(const InputFile &input)
{
    // Reset globals in case samples and real input run in the same process
    devices.clear();
    total = 0;

    const auto &lines = input.getLines();

    // Parse out the lines into a map.
    for (const auto &line : lines)
    {
        const auto parts = common::str::split(line, ':');
        const auto &key = parts[0];
        const auto outputs = common::str::split(parts[1], ' ');
        devices[key] = std::move(outputs);
    }

    findExit(YOU);

    return total;
}