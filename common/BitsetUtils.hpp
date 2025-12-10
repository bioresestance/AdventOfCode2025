#pragma once

#include <bitset>
#include <cstdint>
#include <optional>
#include <vector>

namespace common::bitset_utils
{

/// @brief Find the minimal subset of bitsets that XOR to the target.
/// @param candidates Vector of candidate bitsets
/// @param target The target bitset to achieve via XOR
/// @return Optional vector of indices of selected candidates.
/// Returns nullopt if no solution exists.
template <std::size_t N>
std::optional<std::vector<std::size_t>>
findMinimalXorSubset(const std::vector<std::bitset<N>> &candidates, const std::bitset<N> &target)
{
    const std::size_t n = candidates.size();
    if (n == 0)
    {
        return target.none() ? std::optional<std::vector<std::size_t>>(std::vector<std::size_t>{}) : std::nullopt;
    }
    
    // Brute force: iterate through all 2^n subsets, ordered by popcount (subset size)
    // For small n this is fine; for n > 20 would need a smarter approach
    const uint64_t totalSubsets = 1ULL << n;
    
    // Try subsets in order of increasing size
    for (std::size_t subsetSize = 0; subsetSize <= n; ++subsetSize)
    {
        for (uint64_t mask = 0; mask < totalSubsets; ++mask)
        {
            // Skip if this mask doesn't have exactly subsetSize bits set
            if (static_cast<std::size_t>(__builtin_popcountll(mask)) != subsetSize)
            {
                continue;
            }
            
            // Compute XOR of selected candidates
            std::bitset<N> xorResult;
            for (std::size_t i = 0; i < n; ++i)
            {
                if (mask & (1ULL << i))
                {
                    xorResult ^= candidates[i];
                }
            }
            
            // Check if we found the target
            if (xorResult == target)
            {
                std::vector<std::size_t> indices;
                for (std::size_t i = 0; i < n; ++i)
                {
                    if (mask & (1ULL << i))
                    {
                        indices.push_back(i);
                    }
                }
                return indices;
            }
        }
    }
    
    return std::nullopt;
}

} // namespace common::bitset_utils
