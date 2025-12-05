/**
 * Day-5 - Part 01
 */
#include "include.hpp"
#include <iostream>
#include <ranges>
#include <cstring>


using namespace std::ranges;

int64_t handlePart1(const InputFile &input) {
    const auto &lines = input.getLines();
    
    auto isEmpty = [](const auto& line){return !line.empty();};
    auto ranges = lines | views::take_while(isEmpty) | views::transform([](const std::string& range){
        const auto dashPos = range.find('-');
        const int64_t first = std::strtoll(range.c_str(), nullptr, 10);
        const int64_t second = std::strtoll(range.c_str() + dashPos + 1, nullptr, 10);
        return std::make_pair(first, second);
    });
    auto ids = lines | views::drop_while(isEmpty) | views::drop(1) | views::transform([](const auto id){return std::strtoll(id.c_str(), nullptr, 10);});

    uint32_t total = 0;
    for(const auto id : ids )
    {
        for(const auto [first, second]: ranges)
        {
            if( id >= first and id <= second)
            {
                total++;
                break; // No point checking the other ranges once we satisfy one.
            }
        }
    }

    return total;
}