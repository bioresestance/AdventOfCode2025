#include "Config.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace common
{
namespace
{
constexpr std::string_view kInputEnv = "AOC_INPUT";
constexpr std::string_view kTestsEnv = "AOC_TESTS";
constexpr std::string_view kSkipPart1Env = "AOC_SKIP_PART1";
constexpr std::string_view kSkipPart2Env = "AOC_SKIP_PART2";
constexpr std::string_view kOnlyPartEnv = "AOC_ONLY_PART";
constexpr std::string_view kColorEnv = "AOC_COLOR";

bool parseBoolEnv(const char *value, bool defaultValue)
{
    if (!value)
    {
        return defaultValue;
    }

    std::string_view view(value);
    if (view.empty())
    {
        return defaultValue;
    }

    if (view == "1" || view == "true" || view == "TRUE" || view == "on")
    {
        return true;
    }

    if (view == "0" || view == "false" || view == "FALSE" || view == "off")
    {
        return false;
    }

    return defaultValue;
}

bool isCustomFlag(std::string_view arg)
{
    return arg.starts_with("--input=") || arg == "--input" ||
           arg.starts_with("--test-dir=") || arg == "--test-dir" ||
           arg == "--skip-part1" || arg == "--skip-part2" ||
           arg == "--only-part1" || arg == "--only-part2" ||
           arg == "--no-color" || arg == "--color";
}

void compactArguments(int &argc, char **argv, const std::vector<int> &skipIndices)
{
    if (skipIndices.empty())
    {
        return;
    }
    std::size_t skipIdx = 0;
    int writePos = 1;
    for (int readPos = 1; readPos < argc; ++readPos)
    {
        if (skipIdx < skipIndices.size() && skipIndices[skipIdx] == readPos)
        {
            ++skipIdx;
            continue;
        }
        argv[writePos++] = argv[readPos];
    }
    argc = writePos;
}

std::filesystem::path canonicalizeOrFallback(const std::filesystem::path &candidate,
                                             const std::filesystem::path &fallback)
{
    std::error_code ec;
    const auto canonicalPath = std::filesystem::weakly_canonical(candidate, ec);
    if (!ec && !canonicalPath.empty())
    {
        return canonicalPath;
    }
    return fallback;
}

} // namespace

std::filesystem::path deduceDayDirectory(std::string_view anchorPath)
{
    std::filesystem::path anchor(anchorPath);
    if (anchor.has_filename())
    {
        return anchor.parent_path();
    }
    return std::filesystem::current_path();
}

std::filesystem::path resolveInputPath(std::string_view dayId, std::string_view anchorPath)
{
    auto dir = deduceDayDirectory(anchorPath);
    dir /= "input.txt";
    return dir;
}

std::filesystem::path resolveTestsPath(std::string_view /*dayId*/, std::string_view anchorPath)
{
    auto dir = deduceDayDirectory(anchorPath);
    dir /= "tests";
    return dir;
}

RunOptions buildRunOptions(std::string_view dayId,
                           std::string_view sourcePath,
                           int &argc,
                           char **argv)
{
    RunOptions options;
    options.inputPath = resolveInputPath(dayId, sourcePath);
    options.testsPath = resolveTestsPath(dayId, sourcePath);

    if (const char *envInput = std::getenv(std::string(kInputEnv).c_str()))
    {
        options.inputPath = envInput;
    }
    if (const char *envTests = std::getenv(std::string(kTestsEnv).c_str()))
    {
        options.testsPath = envTests;
    }

    const bool skipPart1Env = parseBoolEnv(std::getenv(std::string(kSkipPart1Env).c_str()), false);
    const bool skipPart2Env = parseBoolEnv(std::getenv(std::string(kSkipPart2Env).c_str()), false);
    const char *onlyPartRaw = std::getenv(std::string(kOnlyPartEnv).c_str());
    if (onlyPartRaw)
    {
        const std::string_view onlyPart(onlyPartRaw);
        if (onlyPart == "1" || onlyPart == "part1")
        {
            options.runPart2 = false;
        }
        else if (onlyPart == "2" || onlyPart == "part2")
        {
            options.runPart1 = false;
        }
    }

    if (skipPart1Env)
    {
        options.runPart1 = false;
    }
    if (skipPart2Env)
    {
        options.runPart2 = false;
    }

    options.colorOutput = parseBoolEnv(std::getenv(std::string(kColorEnv).c_str()), true);

    std::vector<int> consumedArgs;
    for (int i = 1; i < argc; ++i)
    {
        std::string_view arg(argv[i]);
        if (!isCustomFlag(arg))
        {
            continue;
        }

        consumedArgs.push_back(i);
        if (arg.starts_with("--input="))
        {
            options.inputPath = arg.substr(std::string_view("--input=").size());
        }
        else if (arg == "--input")
        {
            if (i + 1 < argc)
            {
                consumedArgs.push_back(i + 1);
                options.inputPath = argv[++i];
            }
            else
            {
                std::cerr << "Missing value for --input flag" << std::endl;
            }
        }
        else if (arg.starts_with("--test-dir="))
        {
            options.testsPath = arg.substr(std::string_view("--test-dir=").size());
        }
        else if (arg == "--test-dir")
        {
            if (i + 1 < argc)
            {
                consumedArgs.push_back(i + 1);
                options.testsPath = argv[++i];
            }
            else
            {
                std::cerr << "Missing value for --test-dir flag" << std::endl;
            }
        }
        else if (arg == "--skip-part1")
        {
            options.runPart1 = false;
        }
        else if (arg == "--skip-part2")
        {
            options.runPart2 = false;
        }
        else if (arg == "--only-part1")
        {
            options.runPart2 = false;
        }
        else if (arg == "--only-part2")
        {
            options.runPart1 = false;
        }
        else if (arg == "--no-color")
        {
            options.colorOutput = false;
        }
        else if (arg == "--color")
        {
            options.colorOutput = true;
        }
    }

    compactArguments(argc, argv, consumedArgs);

    options.inputPath = canonicalizeOrFallback(options.inputPath, resolveInputPath(dayId, sourcePath));
    options.testsPath = canonicalizeOrFallback(options.testsPath, resolveTestsPath(dayId, sourcePath));
    return options;
}

} // namespace common
