#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "Config.hpp"
#include "InputFile.hpp"
#include "ResultAdapter.hpp"

namespace common
{
namespace detail
{
int runDayWithAdapters(int argc,
                       char **argv,
                       std::string_view dayId,
                       std::string_view sourcePath,
                       std::function<std::string(const InputFile &)> part1,
                       std::function<std::string(const InputFile &)> part2);
}

template <typename Part1Fn, typename Part2Fn>
int runDay(int argc,
           char **argv,
           std::string_view dayId,
           std::string_view sourcePath,
           Part1Fn &&part1,
           Part2Fn &&part2)
{
    auto part1Adapter = [part1Fn = std::forward<Part1Fn>(part1)](const InputFile &input) {
        return detail::normalizeResult(part1Fn(input));
    };
    auto part2Adapter = [part2Fn = std::forward<Part2Fn>(part2)](const InputFile &input) {
        return detail::normalizeResult(part2Fn(input));
    };
    return detail::runDayWithAdapters(argc,
                                      argv,
                                      dayId,
                                      sourcePath,
                                      std::move(part1Adapter),
                                      std::move(part2Adapter));
}

} // namespace common
