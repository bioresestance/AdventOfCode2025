#pragma once

#include <functional>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace common::search
{
template <typename Node, typename NeighborFn>
std::unordered_map<Node, int> bfs(const NeighborFn &neighbors, const Node &start)
{
    std::unordered_map<Node, int> distances;
    std::queue<Node> frontier;
    frontier.push(start);
    distances[start] = 0;

    while (!frontier.empty())
    {
        Node current = frontier.front();
        frontier.pop();
        const int nextDistance = distances[current] + 1;
        for (const Node &neighbor : neighbors(current))
        {
            if (distances.contains(neighbor))
            {
                continue;
            }
            distances[neighbor] = nextDistance;
            frontier.push(neighbor);
        }
    }
    return distances;
}

template <typename Node, typename Cost, typename NeighborFn>
std::unordered_map<Node, Cost> dijkstra(const NeighborFn &neighbors, const Node &start)
{
    struct QueueEntry
    {
        Cost cost;
        Node node;
        bool operator>(const QueueEntry &other) const noexcept
        {
            return cost > other.cost;
        }
    };

    std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> frontier;
    std::unordered_map<Node, Cost> distances;
    frontier.push({Cost{}, start});
    distances[start] = Cost{};

    while (!frontier.empty())
    {
        const auto [currentCost, currentNode] = frontier.top();
        frontier.pop();
        if (currentCost != distances[currentNode])
        {
            continue;
        }
        for (const auto &[neighbor, weight] : neighbors(currentNode))
        {
            const Cost candidate = currentCost + weight;
            auto [it, inserted] = distances.emplace(neighbor, candidate);
            if (inserted || candidate < it->second)
            {
                it->second = candidate;
                frontier.push({candidate, neighbor});
            }
        }
    }
    return distances;
}

} // namespace common::search
