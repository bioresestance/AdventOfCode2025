#include <string>
#include <iostream>
#include <string_view>
#include <gtest/gtest.h>
#include <filesystem>
#include "InputFile.hpp"
#include "src/include.hpp"

// Example taken from the puzzle description
const std::string_view exampleInputString = R"(L68
L30
R48
L5
R60
L55
L1
L99
R14
L82)";
const std::string_view testOutputPart1String = "3";
const std::string_view testOutputPart2String = "6";

// Placeholder for the real puzzle answers; update as parts are solved
const std::string_view puzzleOutputPart1String = "1052";
const std::string_view puzzleOutputPart2String = "6295";

class Day1Tests : public ::testing::Test
{
protected:

    InputFile inputFile;

    Day1Tests() : inputFile(getInputFilePath())
    {

    }
   
   static std::string getInputFilePath()
    {
        using std::filesystem::path;

        // Get the directory of the current file
        path currentFilePath = __FILE__;
        path currentDir = currentFilePath.parent_path();

        // Append the input file name to the directory path
        path inputFilePath = currentDir / "input.txt";

        return inputFilePath.string();
    }
};



TEST(Day1Examples, SamplePart1)
{
    EXPECT_EQ(handlePart1(exampleInputString), std::string(testOutputPart1String));
}

TEST(Day1Examples, SamplePart2)
{
    EXPECT_EQ(handlePart2(exampleInputString), std::string(testOutputPart2String));
}


TEST_F(Day1Tests, TestPart1)
{
    const auto result = handlePart1(inputFile.getText());
    const std::string expected{puzzleOutputPart1String};
    EXPECT_EQ(result, expected);
    if (result == expected)
    {
        std::cout << "Day 01 - Part 1 answer: " << result << '\n';
    }
}

TEST_F(Day1Tests, TestPart2)
{
    const auto result = handlePart2(inputFile.getText());
    const std::string expected{puzzleOutputPart2String};
    EXPECT_EQ(result, expected);
    if (result == expected)
    {
        std::cout << "Day 01 - Part 2 answer: " << result << '\n';
    }
}