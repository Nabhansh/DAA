
#include "scheduler.hpp"
#include "dfs_scheduler.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

using Clock = std::chrono::steady_clock;

struct BenchmarkResult {
    long long runtimeUs;
    bool valid;
};

BenchmarkResult runKahn(
    Scheduler& scheduler,
    const std::vector<Job>& jobs) {

    const auto start = Clock::now();
    const std::vector<int> order = scheduler.topologicalSort(jobs);
    const auto finish = Clock::now();

    const auto elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(
            finish - start).count();

    const int n = static_cast<int>(jobs.size());
    std::vector<int> position(n + 1, -1);
    bool valid = order.size() == jobs.size();

    if (valid) {
        for (int i = 0; i < n; ++i) {
            const int id = order[i];

            if (id < 1 || id > n || position[id] != -1) {
                valid = false;
                break;
            }

            position[id] = i;
        }
    }

    if (valid) {
        for (const Job& job : jobs) {
            for (int dependency : job.dependencies) {
                if (dependency < 1 || dependency > n ||
                    position[dependency] >= position[job.id]) {
                    valid = false;
                    break;
                }
            }

            if (!valid) break;
        }
    }

    return {elapsed, valid};
}

BenchmarkResult runDfs(
    DfsScheduler& scheduler,
    const std::vector<Job>& jobs) {

    const auto start = Clock::now();
    const std::vector<int> order = scheduler.topologicalSort(jobs);
    const auto finish = Clock::now();

    const auto elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(
            finish - start).count();

    const int n = static_cast<int>(jobs.size());
    std::vector<int> position(n + 1, -1);
    bool valid = order.size() == jobs.size();

    if (valid) {
        for (int i = 0; i < n; ++i) {
            const int id = order[i];

            if (id < 1 || id > n || position[id] != -1) {
                valid = false;
                break;
            }

            position[id] = i;
        }
    }

    if (valid) {
        for (const Job& job : jobs) {
            for (int dependency : job.dependencies) {
                if (dependency < 1 || dependency > n ||
                    position[dependency] >= position[job.id]) {
                    valid = false;
                    break;
                }
            }

            if (!valid) break;
        }
    }

    return {elapsed, valid};
}

int main() {
    Scheduler kahn;
    DfsScheduler dfs;

    const std::vector<int> sizes = {100, 1000, 5000, 10000};

    std::cout << "Algorithm,Jobs,Dependencies,Runtime_us,Valid\n";

    for (int n : sizes) {
        std::vector<Job> jobs;
        jobs.reserve(n);

        // Generate one reproducible graph for both algorithms.
        std::mt19937 rng(42);
        std::uniform_int_distribution<int> dependencyCount(0, 3);

        long long edgeCount = 0;

        for (int i = 0; i < n; ++i) {
            Job job{i + 1, "tile_" + std::to_string(i + 1), 10, {}};

            if (i > 0) {
                const int count = std::min(dependencyCount(rng), i);

                for (int k = 0; k < count; ++k) {
                    std::uniform_int_distribution<int> earlier(1, i);
                    const int dependency = earlier(rng);
                    job.dependencies.push_back(dependency);
                }

                edgeCount += static_cast<long long>(
                    job.dependencies.size());
            }

            jobs.push_back(std::move(job));
        }

        try {
            const BenchmarkResult k = runKahn(kahn, jobs);
            const BenchmarkResult d = runDfs(dfs, jobs);

            std::cout << "Kahn," << n << ',' << edgeCount << ','
                      << k.runtimeUs << ','
                      << (k.valid ? "yes" : "no") << '\n';

            std::cout << "DFS," << n << ',' << edgeCount << ','
                      << d.runtimeUs << ','
                      << (d.valid ? "yes" : "no") << '\n';

            if (!k.valid || !d.valid) {
                std::cerr << "Invalid topological order detected\n";
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Benchmark failed for " << n
                      << " jobs: " << e.what() << '\n';
            return 1;
        }
    }

    return 0;
}