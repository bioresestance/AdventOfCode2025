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
                       std::function<std::string(const InputFile &)> part1,
                       std::function<std::string(const InputFile &)> part2)
{
    RunOptions options = buildRunOptions(dayId, sourcePath, argc, argv);
    tests::setTestsRoot(options.testsPath);
    tests::setEnabledParts(options.runPart1, options.runPart2);

    ::testing::InitGoogleTest(&argc, argv);
    const bool listingTests = ::testing::GTEST_FLAG(list_tests);
    const int testResult = RUN_ALL_TESTS();
    if (listingTests)
    {
        // CTest invokes gtest binaries with --gtest_list_tests during discovery;
        // bail out early so the actual puzzle execution does not run at build time.
        return testResult;
    }
    if (testResult != 0)
    {
        return testResult;
    }

    InputFile input(options.inputPath.string());

    if (!options.runPart1 && !options.runPart2)
    {
        std::cout << "All parts disabled; nothing to run." << std::endl;
        return 0;
    }

    if (options.runPart1)
    {
        const auto start = Clock::now();
        const auto result = part1(input);
        const auto end = Clock::now();
        const std::chrono::duration<double> elapsed = end - start;
        printResult("Part 1", result, elapsed.count(), options.colorOutput);
    }

    if (options.runPart2)
    {
        const auto start = Clock::now();
        const auto result = part2(input);
        const auto end = Clock::now();
        const std::chrono::duration<double> elapsed = end - start;
        printResult("Part 2", result, elapsed.count(), options.colorOutput);
    }

    return 0;
}

} // namespace detail
} // namespace common
