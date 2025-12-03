#include "Runner.hpp"

#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>

#include <gtest/gtest.h>

#include "InputFile.hpp"
#include "TestHarness.hpp"

namespace common
{
namespace detail
{
namespace
{
using Clock = std::chrono::high_resolution_clock;

std::string colorize(const std::string &text, bool color, std::string_view code)
{
    if (!color)
    {
        return text;
    }
    return std::string("\033[") + std::string(code) + "m" + text + "\033[0m";
}

void printResult(std::string_view label,
                 const std::string &result,
                 double durationSeconds,
                 bool color)
{
    std::cout << label << ": " << colorize(result, color, "32")
              << " (" << durationSeconds << "s)" << std::endl;
}

} // namespace

int runDayWithAdapters(int argc,
                       char **argv,
                       std::string_view dayId,
                       std::string_view sourcePath,
                       std::function<std::string(const std::vector<std::string> &)> part1,
                       std::function<std::string(const std::vector<std::string> &)> part2)
{
    RunOptions options = buildRunOptions(dayId, sourcePath, argc, argv);
    tests::setTestsRoot(options.testsPath);

    ::testing::InitGoogleTest(&argc, argv);
    const int testResult = RUN_ALL_TESTS();
    if (testResult != 0)
    {
        return testResult;
    }

    InputFile input(options.inputPath.string());
    const auto &lines = input.getLines();

    if (!options.runPart1 && !options.runPart2)
    {
        std::cout << "All parts disabled; nothing to run." << std::endl;
        return 0;
    }

    if (options.runPart1)
    {
        const auto start = Clock::now();
        const auto result = part1(lines);
        const auto end = Clock::now();
        const std::chrono::duration<double> elapsed = end - start;
        printResult("Part 1", result, elapsed.count(), options.colorOutput);
    }

    if (options.runPart2)
    {
        const auto start = Clock::now();
        const auto result = part2(lines);
        const auto end = Clock::now();
        const std::chrono::duration<double> elapsed = end - start;
        printResult("Part 2", result, elapsed.count(), options.colorOutput);
    }

    return 0;
}

} // namespace detail
} // namespace common
