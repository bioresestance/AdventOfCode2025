/**
 * Day-6 - Part 02
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

int64_t handlePart2(const InputFile &input) {
    const auto &lines = input.getLines();

    std::vector<Problem> probs;
    uint64_t total = 0;

    auto numberLines = lines | views::take(lines.size() - 1);
    const uint32_t numLines = numberLines.size();
    uint32_t probIdx = 0;
    
    auto ops = lines[lines.size()-1] | views::filter([](const auto& character){return character != ' ';});
    probs.resize(std::ranges::distance(ops));


    
    // Add all of the numbers, which is all lines other than the last, which is the ops.
    for( const auto idx : views::iota(size_t(0), lines[0].size()) | views::reverse)
    {
        std::string colNumber;
        
        for(const auto& line : numberLines)
        {
            colNumber.push_back(line[idx]);
        }
        
        auto numStr = common::str::trim_view(colNumber);
        if(numStr.empty())
        {
            probIdx++;
            continue;
        }
        
        probs[probIdx].numbers.emplace_back(std::strtoll(colNumber.c_str(), nullptr, 10));
        
    }
    
    for(const auto [opNum, op] : ops | views::reverse | views::enumerate)
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