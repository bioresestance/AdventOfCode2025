/**
 * Day-3 - Part 01
 */
#include "include.hpp"
#include <cstdint>
#include <vector>
#include <iostream>

int64_t handlePart1(const InputFile &input) {

    uint64_t total = 0;
    for(const auto& line : input.getLines()) 
    {
        auto nums = common::str::to_vector_of_numbers<uint8_t>(line);
        const auto& [first_digit, first_digit_idx] = common::math::max(nums.begin(), nums.end() - 1); // Find max excluding last
        const auto& [second_digit, second_digit_idx] = common::math::max(nums.begin() + first_digit_idx + 1, nums.end()); // Find max in remaining
        total += static_cast<uint64_t>(first_digit * 10) + static_cast<uint64_t>(second_digit);
    }

    return total;
}