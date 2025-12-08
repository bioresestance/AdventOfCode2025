/**
 * Day-6 - Part 01
 */
#include "include.hpp"
#include <ranges>
using namespace std::ranges;

enum class Operation
{
    ADD,
    MULT,
};

struct Problem
{
    std::vector<int64_t> numbers;
    Operation op;

};

int64_t handlePart1(const InputFile &input) {
    const auto &lines = input.getLines();

    std::vector<Problem> probs;
    uint64_t total = 0;

    // Add all of the numbers, which is all lines other than the last, which is the ops.
    for( const auto& [lineNum, line] : lines | views::take(lines.size() - 1) | views::enumerate)
    {
        auto nums = common::str::to_vector_of_numbers(line, ' ');
        if(probs.size() < nums.size())
        {
            probs.resize(nums.size());
        }

        uint32_t idx = 0;
        for(const auto& num: nums)
        {
            probs[idx++].numbers.emplace_back(num);
        }
    }

    for(const auto [opNum, op] : lines[lines.size()-1] | views::filter([](const auto& character){return character != ' ';}) | views::enumerate)
    {
        probs[opNum].op = (op == '+') ? Operation::ADD : Operation::MULT;
    }

    for(const auto& prob : probs)
    {
        if(prob.op == Operation::ADD)
        {
            uint64_t op_total = 0;
            std::ranges::for_each(prob.numbers, [&op_total](const auto num){op_total += num;});
            total += op_total;
        }
        else
        {
            uint64_t op_total = 1;
            std::ranges::for_each(prob.numbers, [&op_total](const auto num){op_total *= num;});
            total += op_total;
        }
    }
    return total;
}