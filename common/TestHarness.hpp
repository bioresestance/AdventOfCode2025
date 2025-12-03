#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include "ResultAdapter.hpp"
#include "InputFile.hpp"

namespace common::tests
{

enum class Part : uint8_t
{
    One = 1,
    Two = 2
};

struct PartTestCase
{
    std::string name;
    std::vector<std::string> input;
    std::string expected;
};

struct DayTestSuite
{
    std::vector<PartTestCase> part1;
    std::vector<PartTestCase> part2;
};

void setTestsRoot(std::filesystem::path root);
const std::filesystem::path &getTestsRoot();

const DayTestSuite &currentSuite();
const std::vector<PartTestCase> &cases(Part which);

DayTestSuite loadSuiteFrom(const std::filesystem::path &root);

std::vector<std::string> readLines(const std::filesystem::path &path);
std::string readText(const std::filesystem::path &path);

std::filesystem::path resolveCasePath(const std::filesystem::path &root,
                                      Part part,
                                      const std::string &caseName,
                                      std::string_view extension);

InputFile makeInput(const PartTestCase &testCase);

template <typename Solver, typename Expected>
void expect_part(PartTestCase testCase, Solver &&solver, Expected &&expectedOverride)
{
    auto input = makeInput(testCase);
    const auto actual = detail::normalizeResult(solver(input));
    const auto expected = detail::normalizeResult(std::forward<Expected>(expectedOverride));
    EXPECT_EQ(actual, expected) << testCase.name;
}

template <typename Solver>
void expect_part(PartTestCase testCase, Solver &&solver)
{
    auto input = makeInput(testCase);
    const auto actual = detail::normalizeResult(solver(input));
    EXPECT_EQ(actual, testCase.expected) << testCase.name;
}

} // namespace common::tests
