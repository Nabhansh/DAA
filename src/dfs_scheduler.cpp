
#include "dfs_scheduler.hpp"

#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

std::vector<int> DfsScheduler::topologicalSort(
    const std::vector<Job>& jobs) {

    const int n = static_cast<int>(jobs.size());

    std::unordered_map<int, int> indexById;

    for (int i = 0; i < n; ++i) {
        if (!indexById.emplace(jobs[i].id, i).second) {
            throw std::invalid_argument("Duplicate job ID");
        }
    }

    // Store edges from each dependent job to its prerequisites.
    std::vector<std::vector<int>> graph(n);

    for (int i = 0; i < n; ++i) {
        for (int dependencyId : jobs[i].dependencies) {
            auto it = indexById.find(dependencyId);

            if (it == indexById.end()) {
                throw std::invalid_argument("Unknown dependency ID");
            }

            graph[i].push_back(it->second);
        }
    }

    // 0 = unvisited, 1 = visiting, 2 = completed.
    std::vector<int> state(n, 0);
    std::vector<int> order;
    order.reserve(n);

    std::function<void(int)> dfs = [&](int current) {
        if (state[current] == 1) {
            throw std::invalid_argument("Dependency cycle detected");
        }

        if (state[current] == 2) {
            return;
        }

        state[current] = 1;

        for (int prerequisite : graph[current]) {
            dfs(prerequisite);
        }

        state[current] = 2;
        order.push_back(jobs[current].id);
    };

    for (int i = 0; i < n; ++i) {
        if (state[i] == 0) {
            dfs(i);
        }
    }

    return order;
}