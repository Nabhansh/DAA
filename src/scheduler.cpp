
#include "scheduler.hpp"

#include <queue>
#include <unordered_map>

std::vector<int> Scheduler::topologicalSort(const std::vector<Job>& jobs) {
    const int n = static_cast<int>(jobs.size());

    std::unordered_map<int, int> indexById;

    for (int i = 0; i < n; ++i) {
        if (!indexById.emplace(jobs[i].id, i).second) {
            throw std::invalid_argument("Duplicate job ID");
        }
    }

    std::vector<std::vector<int>> graph(n);
    std::vector<int> indegree(n, 0);

    for (int i = 0; i < n; ++i) {
        for (int dependencyId : jobs[i].dependencies) {
            auto it = indexById.find(dependencyId);

            if (it == indexById.end()) {
                throw std::invalid_argument("Unknown dependency ID");
            }

            int prerequisite = it->second;
            graph[prerequisite].push_back(i);
            ++indegree[i];
        }
    }

    std::queue<int> ready;

    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0) {
            ready.push(i);
        }
    }

    std::vector<int> result;
    result.reserve(n);

    while (!ready.empty()) {
        int current = ready.front();
        ready.pop();

        result.push_back(jobs[current].id);

        for (int dependent : graph[current]) {
            --indegree[dependent];

            if (indegree[dependent] == 0) {
                ready.push(dependent);
            }
        }
    }

    if (static_cast<int>(result.size()) != n) {
        throw std::invalid_argument("Dependency cycle detected");
    }

    return result;
}