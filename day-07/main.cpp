#include <gtest/gtest.h>

#include <string_view>

#include "Runner.hpp"
#include "TestHarness.hpp"
#include "src/include.hpp"

namespace {
constexpr std::string_view kDayId = "07";
constexpr std::string_view kSourcePath = __FILE__;
}

namespace {
void runSampleSuite(common::tests::Part part)
{
    if (!common::tests::isPartEnabled(part))
    {
        GTEST_SKIP() << "Part disabled by CLI flags.";
    }
    const auto &cases = common::tests::cases(part);
    if (cases.empty())
    {
        GTEST_SKIP() << "No sample cases provided for this part yet.";
    }

    for (const auto &testCase : cases)
    {
        if (part == common::tests::Part::One)
        {
            if (testCase.expected.empty())
            {
                auto inputFile = common::tests::makeInput(testCase);
                handlePart1(inputFile);
                continue;
            }
            common::tests::expect_part(testCase, handlePart1);
        }
        else
        {
            if (testCase.expected.empty())
            {
                auto inputFile = common::tests::makeInput(testCase);
                handlePart2(inputFile);
                continue;
            }
            common::tests::expect_part(testCase, handlePart2);
        }
    }
}
} // namespace

TEST(Day7Part1Samples, ValidateExamples)
{
    runSampleSuite(common::tests::Part::One);
}

TEST(Day7Part2Samples, ValidateExamples)
{
    runSampleSuite(common::tests::Part::Two);
}

int main(int argc, char **argv)
{
    return common::runDay(argc, argv, kDayId, kSourcePath, handlePart1, handlePart2);
}