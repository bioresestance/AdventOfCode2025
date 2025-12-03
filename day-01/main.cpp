#include <gtest/gtest.h>

#include <string_view>

#include "Config.hpp"
#include "InputFile.hpp"
#include "Runner.hpp"
#include "TestHarness.hpp"
#include "src/include.hpp"

namespace
{
constexpr std::string_view kDayId = "01";
constexpr std::string_view kSourcePath = __FILE__;

void runSampleSuite(common::tests::Part part)
{
    const auto &cases = common::tests::cases(part);
    if (cases.empty())
    {
        GTEST_SKIP() << "No sample cases provided for this part yet.";
    }

    for (const auto &testCase : cases)
    {
        if (part == common::tests::Part::One)
        {
            common::tests::expect_part(testCase, handlePart1);
        }
        else
        {
            common::tests::expect_part(testCase, handlePart2);
        }
    }
}

InputFile loadPuzzleInput()
{
    const auto inputPath = common::resolveInputPath(kDayId, kSourcePath);
    return InputFile(inputPath.string());
}
} // namespace

TEST(Day01Part1Samples, ValidateExamples)
{
    runSampleSuite(common::tests::Part::One);
}

TEST(Day01Part2Samples, ValidateExamples)
{
    runSampleSuite(common::tests::Part::Two);
}

TEST(Day01Puzzle, Part1Answer)
{
    auto input = loadPuzzleInput();
    EXPECT_EQ(handlePart1(input.getLines()), 1052);
}

TEST(Day01Puzzle, Part2Answer)
{
    auto input = loadPuzzleInput();
    EXPECT_EQ(handlePart2(input.getLines()), 6295);
}

int main(int argc, char **argv)
{
    return common::runDay(argc, argv, kDayId, kSourcePath, handlePart1, handlePart2);
}