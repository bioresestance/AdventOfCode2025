#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace common
{
struct RunOptions
{
    bool runPart1 = true;
    bool runPart2 = true;
    bool colorOutput = true;
    std::filesystem::path inputPath;
    std::filesystem::path testsPath;
};

/**
 * @brief Parse command-line arguments and environment overrides for the given day.
 *        Custom Advent of Code flags are removed from argc/argv so GoogleTest ignores them.
 */
RunOptions buildRunOptions(std::string_view dayId,
                           std::string_view sourcePath,
                           int &argc,
                           char **argv);

/// Returns the default input path for the provided day based on the executable location.
std::filesystem::path resolveInputPath(std::string_view dayId, std::string_view anchorPath);

/// Returns the default test directory for the provided day based on the executable location.
std::filesystem::path resolveTestsPath(std::string_view dayId, std::string_view anchorPath);

/// Returns the directory that contains the calling source file (useful for tests).
std::filesystem::path deduceDayDirectory(std::string_view anchorPath);
}
