/**
 * Day-10 - Part 01
 */
#include "include.hpp"
#include <ranges>
#include <iostream>
#include <bitset>

using namespace std::ranges;

struct Machine
{
    std::bitset<16> lights;
    std::vector<std::bitset<16>> buttons;
    std::vector<uint32_t> joltages;
};

Machine parseMachineLine(const std::string &line)
{
    Machine newMachine;
    auto values = common::str::split(line, ' ');

    for (const auto &val : values)
    {

        if (val.starts_with('['))
        {
            auto bitsStr = common::str::remove_chars(val, "[]");
            // Reverse the string because bitset parses left-to-right as MSB-to-LSB,
            // but the puzzle uses left-to-right as bit 0 to bit N
            std::string reversed(bitsStr.rbegin(), bitsStr.rend());
            newMachine.lights = std::bitset<16>(reversed, 0, reversed.size(), '.', '#');
        }
        else if (val.starts_with('('))
        {
            const auto buttonsStr = common::str::remove_chars(val, "()");
            auto buttons = common::str::to_vector_of_numbers(buttonsStr, ',');
            std::bitset<16> buttonBits;
            std::ranges::for_each(buttons, [&](auto button)
                                  { buttonBits.set(button); });
            newMachine.buttons.emplace_back(std::move(buttonBits));
        }
        else if (val.starts_with('{'))
        {
            continue; // Part 1 is ignoring this part.
        }
    }
    return newMachine;
}

int64_t handlePart1(const InputFile &input)
{
    const auto &lines = input.getLines();

    std::vector<Machine> machines;

    for (const auto &line : lines)
    {
        auto newMachine = parseMachineLine(line);
        machines.emplace_back(std::move(newMachine));
    }

    uint64_t total = 0;

    for (const auto &machine : machines)
    {
        auto result = common::bitset_utils::findMinimalXorSubset(machine.buttons, machine.lights);
        if (result)
        {
            total += result->size();
        }
        else
        {
            std::cout << "Failed to find answer!!!!" << std::endl;
        }
    }

    return total;
}