#include "TestHarness.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <mutex>
#include <optional>
#include <stdexcept>

namespace common::tests
{
namespace
{
std::filesystem::path g_testsRoot;
std::mutex g_cacheMutex;
std::optional<DayTestSuite> g_cachedSuite;

std::string_view partFolder(Part part)
{
    return part == Part::One ? std::string_view("part1") : std::string_view("part2");
}

std::string normalizeNewlines(std::string value)
{
    while (!value.empty() && (value.back() == '\n' || value.back() == '\r'))
    {
        value.pop_back();
    }
    return value;
}

std::vector<PartTestCase> loadPartCases(const std::filesystem::path &root, Part part)
{
    std::vector<PartTestCase> cases;
    const auto partDir = root / partFolder(part);
    if (!std::filesystem::exists(partDir))
    {
        return cases;
    }

    for (const auto &entry : std::filesystem::directory_iterator(partDir))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".input")
        {
            continue;
        }

        PartTestCase testCase;
        testCase.name = entry.path().stem().string();
        testCase.input = readLines(entry.path());
        auto expectedPath = entry.path();
        expectedPath.replace_extension(".expected");
        if (std::filesystem::exists(expectedPath))
        {
            testCase.expected = normalizeNewlines(readText(expectedPath));
        }
        cases.emplace_back(std::move(testCase));
    }

    std::sort(cases.begin(), cases.end(), [](const PartTestCase &lhs, const PartTestCase &rhs) {
        return lhs.name < rhs.name;
    });

    return cases;
}

} // namespace

void setTestsRoot(std::filesystem::path root)
{
    std::scoped_lock lock(g_cacheMutex);
    g_testsRoot = std::move(root);
    g_cachedSuite.reset();
}

const std::filesystem::path &getTestsRoot()
{
    if (g_testsRoot.empty())
    {
        g_testsRoot = std::filesystem::current_path() / "tests";
    }
    return g_testsRoot;
}

const DayTestSuite &currentSuite()
{
    {
        std::scoped_lock lock(g_cacheMutex);
        if (g_cachedSuite.has_value())
        {
            return *g_cachedSuite;
        }
    }

    const auto root = getTestsRoot();
    DayTestSuite loaded = loadSuiteFrom(root);

    std::scoped_lock lock(g_cacheMutex);
    if (!g_cachedSuite.has_value())
    {
        g_cachedSuite = std::move(loaded);
    }
    return *g_cachedSuite;
}

const std::vector<PartTestCase> &cases(Part which)
{
    const auto &suite = currentSuite();
    return which == Part::One ? suite.part1 : suite.part2;
}

DayTestSuite loadSuiteFrom(const std::filesystem::path &root)
{
    DayTestSuite suite;
    suite.part1 = loadPartCases(root, Part::One);
    suite.part2 = loadPartCases(root, Part::Two);
    return suite;
}

std::vector<std::string> readLines(const std::filesystem::path &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open test input: " + path.string());
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        lines.push_back(line);
    }
    if (!file.eof())
    {
        throw std::runtime_error("Error while reading test input: " + path.string());
    }
    return lines;
}

std::string readText(const std::filesystem::path &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open expected output: " + path.string());
    }
    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return contents;
}

std::filesystem::path resolveCasePath(const std::filesystem::path &root,
                                      Part part,
                                      const std::string &caseName,
                                      std::string_view extension)
{
    auto dir = root / partFolder(part);
    std::filesystem::create_directories(dir);
    auto path = dir / caseName;
    path += extension;
    return path;
}

InputFile makeInput(const PartTestCase &testCase)
{
    return InputFile::fromLines(testCase.input, testCase.name);
}

} // namespace common::tests
