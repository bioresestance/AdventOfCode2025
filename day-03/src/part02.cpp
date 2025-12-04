/**
 * Day-3 - Part 02
 */
#include "include.hpp"
#include <array>

int64_t handlePart2(const InputFile &input) 
{
    constexpr std::size_t NUM_DIGITS = 12;
    
    uint64_t total = 0;
    for(const auto& line : input.getLines())
    {
        std::array<char, NUM_DIGITS + 1> digits{0};
        auto nums = common::str::to_vector_of_numbers<uint8_t>(line);
        std::size_t start_idx = 0;

        for(std::size_t i = 0; i < NUM_DIGITS; i++)
        {
            // For position i, we need to leave (NUM_DIGITS - 1 - i) positions after this one
            // So the last valid index we can pick from is: nums.size() - (NUM_DIGITS - 1 - i) - 1
            // = nums.size() - NUM_DIGITS + i
            std::size_t remaining_positions = NUM_DIGITS - 1 - i;
            std::size_t end_idx = nums.size() - remaining_positions;
            
            const auto& [digit, digit_idx] = common::math::max(nums.begin() + start_idx, nums.begin() + end_idx);
            start_idx = start_idx + digit_idx + 1; // Move past the selected digit
            digits[i] = digit + '0';
        }
        total += std::strtoll(digits.data(), nullptr, 10);
    }

    return total;
}