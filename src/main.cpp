
#include "scheduler.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    std::cout << "Project 47: Satellite Image Tile Processing Pipeline Scheduler\n\n";

    std::ifstream input("data/jobs.txt");

    if (!input) {
        std::cerr << "Error: Cannot open data/jobs.txt\n";
        return 1;
    }

    int jobCount;

    if (!(input >> jobCount) || jobCount < 0) {
        std::cerr << "Error: Invalid job count\n";
        return 1;
    }

    std::vector<Job> jobs;
    jobs.reserve(static_cast<std::size_t>(jobCount));

    for (int i = 0; i < jobCount; ++i) {
        Job job{};
        int dependencyCount;

        if (!(input >> job.id >> job.tile_name
                    >> job.processing_cost >> dependencyCount) ||
            dependencyCount < 0 || job.processing_cost < 0) {
            std::cerr << "Error: Invalid job record\n";
            return 1;
        }

        for (int j = 0; j < dependencyCount; ++j) {
            int dependencyId;

            if (!(input >> dependencyId)) {
                std::cerr << "Error: Missing dependency ID\n";
                return 1;
            }

            job.dependencies.push_back(dependencyId);
        }

        jobs.push_back(job);
    }

    Scheduler scheduler;

    try {
        const std::vector<int> order = scheduler.topologicalSort(jobs);

        std::cout << "Satellite Tile Processing Order:\n";

        for (int id : order) {
            for (const Job& job : jobs) {
                if (job.id == id) {
                    std::cout << "Job " << job.id
                              << " | Tile: " << job.tile_name
                              << " | Cost: " << job.processing_cost
                              << '\n';
                    break;
                }
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Scheduling failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}