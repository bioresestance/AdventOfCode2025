/**
 * Day-10 - Part 02
 *
 * This is an Integer Linear Programming problem:
 * Minimize sum(x) subject to Ax = b, x >= 0 (integers)
 *
 * Solution approach:
 * 1. Use Gaussian elimination to find RREF of [A|b]
 * 2. Identify pivot columns (basic variables) and free columns (free variables)
 * 3. Basic variables are uniquely determined by free variables via back-substitution
 * 4. Search over all valid assignments of free variables to find minimum sum
 *
 * The search is efficient because:
 * - AoC inputs have few free variables (typically 0-2)
 * - Pruning eliminates branches where current sum exceeds best known
 * - Feasibility constraints limit the search space
 */
#include "include.hpp"
#include <ranges>
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>
#include <queue>
#include <set>
#include <Eigen/Dense>

using namespace std::ranges;

struct MachinePart2
{
    std::vector<std::vector<uint32_t>> buttons;
    std::vector<uint32_t> joltages;
};

static MachinePart2 parseMachineLine(const std::string &line)
{
    MachinePart2 newMachine;
    auto values = common::str::split(line, ' ');

    for (const auto &val : values)
    {

        if (val.starts_with('['))
        {
            continue; // Part2 ignores this part.
        }
        else if (val.starts_with('('))
        {
            const auto buttonsStr = common::str::remove_chars(val, "()");
            auto buttonCounters = common::str::to_vector_of_numbers<uint32_t>(buttonsStr, ',');
            newMachine.buttons.emplace_back(std::move(buttonCounters));
        }
        else if (val.starts_with('{'))
        {
            const auto joltagesStr = common::str::remove_chars(val, "{}");
            newMachine.joltages = common::str::to_vector_of_numbers<uint32_t>(joltagesStr, ',');
        }
    }
    return newMachine;
}

/**
 * Gaussian elimination with rational arithmetic to find exact solutions.
 * Uses fractions represented as pairs of integers to avoid floating point errors.
 */
struct Rational
{
    int64_t num;
    int64_t den;

    Rational(int64_t n = 0, int64_t d = 1) : num(n), den(d) { normalize(); }

    void normalize()
    {
        if (den < 0)
        {
            num = -num;
            den = -den;
        }
        int64_t g = std::gcd(std::abs(num), std::abs(den));
        if (g > 0)
        {
            num /= g;
            den /= g;
        }
    }

    Rational operator+(const Rational &other) const
    {
        return Rational(num * other.den + other.num * den, den * other.den);
    }

    Rational operator-(const Rational &other) const
    {
        return Rational(num * other.den - other.num * den, den * other.den);
    }

    Rational operator*(const Rational &other) const
    {
        return Rational(num * other.num, den * other.den);
    }

    Rational operator/(const Rational &other) const
    {
        return Rational(num * other.den, den * other.num);
    }

    bool isZero() const { return num == 0; }
    bool isPositive() const { return num > 0; }
    bool isNegative() const { return num < 0; }

    double toDouble() const { return static_cast<double>(num) / static_cast<double>(den); }
    int64_t toInt() const { return num / den; } // Only valid if den divides num
    bool isInteger() const { return num % den == 0; }
};

struct RationalGaussResult
{
    std::vector<std::vector<Rational>> rref;
    std::vector<int> pivotCols;
    int rank;
};

static RationalGaussResult gaussianElimination(const Eigen::MatrixXd &A, const Eigen::VectorXd &b)
{
    int rows = A.rows();
    int cols = A.cols();

    // Create augmented matrix [A|b] with rationals
    std::vector<std::vector<Rational>> aug(rows, std::vector<Rational>(cols + 1));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            aug[i][j] = Rational(static_cast<int64_t>(A(i, j)));
        }
        aug[i][cols] = Rational(static_cast<int64_t>(b(i)));
    }

    std::vector<int> pivotCols;
    int pivotRow = 0;

    for (int col = 0; col < cols && pivotRow < rows; ++col)
    {
        // Find non-zero pivot
        int foundRow = -1;
        for (int row = pivotRow; row < rows; ++row)
        {
            if (!aug[row][col].isZero())
            {
                foundRow = row;
                break;
            }
        }

        if (foundRow == -1)
        {
            continue; // Skip this column
        }

        // Swap rows
        std::swap(aug[pivotRow], aug[foundRow]);

        // Scale pivot row to make pivot = 1
        Rational pivot = aug[pivotRow][col];
        for (int j = 0; j <= cols; ++j)
        {
            aug[pivotRow][j] = aug[pivotRow][j] / pivot;
        }

        // Eliminate other rows
        for (int row = 0; row < rows; ++row)
        {
            if (row != pivotRow && !aug[row][col].isZero())
            {
                Rational factor = aug[row][col];
                for (int j = 0; j <= cols; ++j)
                {
                    aug[row][j] = aug[row][j] - factor * aug[pivotRow][j];
                }
            }
        }

        pivotCols.push_back(col);
        pivotRow++;
    }

    return {aug, pivotCols, static_cast<int>(pivotCols.size())};
}
/**
 * Solve using RREF and then optimize over free variables.
 */
static int64_t solveMachine(const MachinePart2 &machine)
{
    const int numCounters = static_cast<int>(machine.joltages.size());
    const int numButtons = static_cast<int>(machine.buttons.size());

    // Build the coefficient matrix A (counters x buttons)
    Eigen::MatrixXd A(numCounters, numButtons);
    A.setZero();

    for (int btnIdx = 0; btnIdx < numButtons; ++btnIdx)
    {
        for (uint32_t counterIdx : machine.buttons[btnIdx])
        {
            if (static_cast<int>(counterIdx) < numCounters)
            {
                A(counterIdx, btnIdx) = 1.0;
            }
        }
    }

    // Build the target vector b
    Eigen::VectorXd b(numCounters);
    for (int i = 0; i < numCounters; ++i)
    {
        b(i) = static_cast<double>(machine.joltages[i]);
    }

    // Gaussian elimination
    auto result = gaussianElimination(A, b);
    const auto &rref = result.rref;
    const auto &pivotCols = result.pivotCols;
    int rank = result.rank;

    // Identify free variables (non-pivot columns)
    std::vector<int> freeCols;
    std::set<int> pivotSet(pivotCols.begin(), pivotCols.end());
    for (int col = 0; col < numButtons; ++col)
    {
        if (pivotSet.find(col) == pivotSet.end())
        {
            freeCols.push_back(col);
        }
    }

    // Debug: print dimensions (comment out for production)
    std::cerr << "Counters: " << numCounters << ", Buttons: " << numButtons
              << ", Rank: " << rank << ", Free vars: " << freeCols.size() << std::endl;

    // For each assignment of free variables, compute basic variables
    // and check feasibility (all >= 0)
    // Then find minimum sum

    // If no free variables, we have a unique solution
    if (freeCols.empty())
    {
        // The solution is: x[pivotCols[i]] = rref[i][numButtons] for i in 0..rank-1
        // Other variables (if numButtons > rank) are 0
        std::vector<int64_t> solution(numButtons, 0);
        int64_t totalPresses = 0;
        bool valid = true;

        for (int i = 0; i < rank; ++i)
        {
            Rational val = rref[i][numButtons];
            if (!val.isInteger())
            {
                std::cerr << "Warning: Non-integer solution for button " << pivotCols[i] << std::endl;
                valid = false;
            }
            int64_t presses = val.toInt();
            if (presses < 0)
            {
                std::cerr << "Warning: Negative presses in unique solution for button " << pivotCols[i] << std::endl;
                valid = false;
            }
            solution[pivotCols[i]] = presses;
            totalPresses += presses;
        }

        // Verify the solution
        std::vector<int64_t> counters(numCounters, 0);
        for (int btnIdx = 0; btnIdx < numButtons; ++btnIdx)
        {
            for (uint32_t counterIdx : machine.buttons[btnIdx])
            {
                if (static_cast<int>(counterIdx) < numCounters)
                {
                    counters[counterIdx] += solution[btnIdx];
                }
            }
        }

        for (int i = 0; i < numCounters; ++i)
        {
            if (counters[i] != static_cast<int64_t>(machine.joltages[i]))
            {
                std::cerr << "ERROR (unique): Counter " << i << " is " << counters[i]
                          << " but should be " << machine.joltages[i] << std::endl;
                valid = false;
            }
        }

        if (!valid)
        {
            std::cerr << "Unique solution verification failed!" << std::endl;
        }

        return totalPresses;
    }

    // We have free variables - need to search over them
    // For each free variable, determine its valid range

    // Use BFS/DFS to search over free variable assignments
    // This is exponential in the number of free variables, but AoC
    // problems are designed to be tractable
    // Compute upper bounds for free variables
    // A simple bound: the free variable can't exceed the maximum RHS value
    // since pressing a button more times than needed is wasteful
    int maxJoltage = *std::max_element(machine.joltages.begin(), machine.joltages.end());
    std::vector<int> freeUpperBounds(freeCols.size(), maxJoltage);

    // BFS over free variable space
    int64_t bestSum = std::numeric_limits<int64_t>::max();

    // Helper to compute solution and check feasibility
    auto computeSolution = [&](const std::vector<int64_t> &freeVals) -> std::tuple<bool, int64_t, std::vector<int64_t>>
    {
        std::vector<int64_t> solution(numButtons, 0);

        // Set free variables
        for (size_t i = 0; i < freeCols.size(); ++i)
        {
            solution[freeCols[i]] = freeVals[i];
        }

        // Compute basic variables from RREF (back-substitution)
        for (int i = rank - 1; i >= 0; --i)
        {
            Rational val = rref[i][numButtons];
            for (int j = pivotCols[i] + 1; j < numButtons; ++j)
            {
                val = val - rref[i][j] * Rational(solution[j]);
            }
            if (!val.isInteger())
            {
                return {false, 0, {}};
            }
            int64_t intVal = val.toInt();
            if (intVal < 0)
            {
                return {false, 0, {}};
            }
            solution[pivotCols[i]] = intVal;
        }

        int64_t totalSum = std::accumulate(solution.begin(), solution.end(), 0LL);
        return {true, totalSum, solution};
    };

    std::vector<int64_t> bestSolution;

    // Use DFS to search over free variable assignments (no early pruning on currentFreeSum)
    std::function<void(int, std::vector<int64_t> &)> search;
    search = [&](int freeIdx, std::vector<int64_t> &freeVals)
    {
        if (freeIdx == static_cast<int>(freeCols.size()))
        {
            auto [feasible, totalSum, solution] = computeSolution(freeVals);
            if (feasible && totalSum < bestSum)
            {
                bestSum = totalSum;
                bestSolution = solution;
            }
            return;
        }

        // Try values for this free variable from 0 to upper bound
        for (int64_t val = 0; val <= freeUpperBounds[freeIdx]; ++val)
        {
            freeVals[freeIdx] = val;
            search(freeIdx + 1, freeVals);
        }
    };

    std::vector<int64_t> freeVals(freeCols.size(), 0);
    search(0, freeVals);

    if (bestSum == std::numeric_limits<int64_t>::max())
    {
        std::cerr << "Warning: No feasible solution found!" << std::endl;
        return 0;
    }

    // Verify the solution is correct
    if (!bestSolution.empty())
    {
        std::vector<int64_t> counters(numCounters, 0);
        for (int btnIdx = 0; btnIdx < numButtons; ++btnIdx)
        {
            for (uint32_t counterIdx : machine.buttons[btnIdx])
            {
                if (static_cast<int>(counterIdx) < numCounters)
                {
                    counters[counterIdx] += bestSolution[btnIdx];
                }
            }
        }

        bool valid = true;
        for (int i = 0; i < numCounters; ++i)
        {
            if (counters[i] != static_cast<int64_t>(machine.joltages[i]))
            {
                std::cerr << "ERROR: Counter " << i << " is " << counters[i]
                          << " but should be " << machine.joltages[i] << std::endl;
                valid = false;
            }
        }
        if (!valid)
        {
            std::cerr << "Solution verification failed!" << std::endl;
        }
    }

    return bestSum;
}

int64_t handlePart2(const InputFile &input)
{
    const auto &lines = input.getLines();
    int64_t total = 0;

    int machineNum = 0;
    for (const auto &line : lines)
    {
        auto machine = parseMachineLine(line);
        int64_t machinePresses = solveMachine(machine);
        std::cerr << "Machine " << machineNum++ << ": " << machinePresses << " presses" << std::endl;
        total += machinePresses;
    }
    return total;
}