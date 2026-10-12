
#include "scheduler.hpp"
#include "dfs_scheduler.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    Scheduler scheduler;
    DfsScheduler dfs;

    int passed = 0;
    int total = 0;

    auto check = [&](bool condition, const char* name) {
        ++total;
        if (!condition) {
            std::cerr << "FAIL: " << name << '\n';
            return false;
        }
        std::cout << "PASS: " << name << '\n';
        ++passed;
        return true;
    };

    auto expectInvalid = [&](const std::vector<Job>& jobs,
                             const char* name) {
        try {
            scheduler.topologicalSort(jobs);
        } catch (const std::invalid_argument&) {
            return check(true, name);
        } catch (const std::exception& e) {
            std::cerr << "FAIL: " << name
                      << " (unexpected exception: "
                      << e.what() << ")\n";
            ++total;
            return false;
        }
        return check(false, name);
    };

    auto expectDfsInvalid = [&](const std::vector<Job>& jobs,
                                const char* name) {
        try {
            dfs.topologicalSort(jobs);
        } catch (const std::invalid_argument&) {
            return check(true, name);
        } catch (const std::exception& e) {
            std::cerr << "FAIL: " << name
                      << " (unexpected exception: "
                      << e.what() << ")\n";
            ++total;
            return false;
        }
        return check(false, name);
    };

    auto position = [](const std::vector<int>& order, int id) {
        auto it = std::find(order.begin(), order.end(), id);
        return it == order.end()
            ? -1
            : static_cast<int>(it - order.begin());
    };

    // Kahn test 1: Valid dependency graph
    const std::vector<Job> jobs = {
        {1, "tile_A", 10, {}},
        {2, "tile_B", 15, {}},
        {3, "tile_C", 20, {1, 2}},
        {4, "tile_D", 25, {3}}
    };

    try {
        const auto order = scheduler.topologicalSort(jobs);
        if (!check(
                order.size() == jobs.size() &&
                position(order, 1) >= 0 &&
                position(order, 2) >= 0 &&
                position(order, 3) >= 0 &&
                position(order, 4) >= 0 &&
                position(order, 1) < position(order, 3) &&
                position(order, 2) < position(order, 3) &&
                position(order, 3) < position(order, 4),
                "Kahn: Valid dependency ordering")) {
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Kahn valid graph failed: " << e.what() << '\n';
        return 1;
    }

    // Kahn tests 2-5: Invalid graphs
    if (!expectInvalid({
            {1, "tile_A", 10, {2}},
            {2, "tile_B", 15, {1}}
        }, "Kahn: Cycle rejected")) return 1;

    if (!expectInvalid({
            {1, "tile_A", 10, {99}}
        }, "Kahn: Unknown dependency rejected")) return 1;

    if (!expectInvalid({
            {1, "tile_A", 10, {}},
            {1, "tile_B", 15, {}}
        }, "Kahn: Duplicate IDs rejected")) return 1;

    if (!expectInvalid({
            {1, "tile_A", 10, {1}}
        }, "Kahn: Self-dependency rejected")) return 1;

    // Kahn test 6: Empty graph
    try {
        const auto order = scheduler.topologicalSort({});
        if (!check(order.empty(), "Kahn: Empty graph")) return 1;
    } catch (...) {
        std::cerr << "Kahn: Empty graph unexpectedly failed\n";
        return 1;
    }

    // Kahn test 7: Independent jobs
    try {
        const std::vector<Job> independent = {
            {10, "tile_A", 5, {}},
            {20, "tile_B", 8, {}},
            {30, "tile_C", 12, {}}
        };
        const auto order = scheduler.topologicalSort(independent);
        if (!check(
                order.size() == 3 &&
                position(order, 10) >= 0 &&
                position(order, 20) >= 0 &&
                position(order, 30) >= 0,
                "Kahn: Independent jobs")) return 1;
    } catch (...) {
        std::cerr << "Kahn: Independent jobs unexpectedly failed\n";
        return 1;
    }

    // Kahn test 8: Dependency chain
    try {
        const std::vector<Job> chain = {
            {1, "tile_A", 5, {}},
            {2, "tile_B", 8, {1}},
            {3, "tile_C", 12, {2}}
        };
        const auto order = scheduler.topologicalSort(chain);
        if (!check(
                order.size() == 3 &&
                position(order, 1) < position(order, 2) &&
                position(order, 2) < position(order, 3),
                "Kahn: Dependency chain")) return 1;
    } catch (...) {
        std::cerr << "Kahn: Dependency chain unexpectedly failed\n";
        return 1;
    }

    // DFS test 1: Valid dependency ordering
    try {
        const auto order = dfs.topologicalSort(jobs);
        if (!check(
                order.size() == jobs.size() &&
                position(order, 1) < position(order, 3) &&
                position(order, 2) < position(order, 3) &&
                position(order, 3) < position(order, 4),
                "DFS: Valid dependency ordering")) return 1;
    } catch (const std::exception& e) {
        std::cerr << "DFS valid graph failed: " << e.what() << '\n';
        return 1;
    }

    // DFS test 2: Dependency chain
    try {
        const std::vector<Job> chain = {
            {1, "tile_A", 5, {}},
            {2, "tile_B", 8, {1}},
            {3, "tile_C", 12, {2}}
        };
        const auto order = dfs.topologicalSort(chain);
        if (!check(
                order.size() == 3 &&
                position(order, 1) < position(order, 2) &&
                position(order, 2) < position(order, 3),
                "DFS: Dependency chain")) return 1;
    } catch (...) {
        std::cerr << "DFS: Dependency chain unexpectedly failed\n";
        return 1;
    }

    // DFS test 3: Independent jobs
    try {
        const std::vector<Job> independent = {
            {10, "tile_A", 5, {}},
            {20, "tile_B", 8, {}},
            {30, "tile_C", 12, {}}
        };
        const auto order = dfs.topologicalSort(independent);
        if (!check(
                order.size() == 3 &&
                position(order, 10) >= 0 &&
                position(order, 20) >= 0 &&
                position(order, 30) >= 0,
                "DFS: Independent jobs")) return 1;
    } catch (...) {
        std::cerr << "DFS: Independent jobs unexpectedly failed\n";
        return 1;
    }

    // DFS tests 4-6: Invalid graphs
    if (!expectDfsInvalid({
            {1, "tile_A", 10, {2}},
            {2, "tile_B", 15, {1}}
        }, "DFS: Cycle rejected")) return 1;

    if (!expectDfsInvalid({
            {1, "tile_A", 10, {99}}
        }, "DFS: Unknown dependency rejected")) return 1;

    if (!expectDfsInvalid({
            {1, "tile_A", 10, {}},
            {1, "tile_B", 15, {}}
        }, "DFS: Duplicate IDs rejected")) return 1;

    std::cout << "\n" << passed << "/" << total
              << " tests passed.\n";

    return passed == total ? 0 : 1;
}