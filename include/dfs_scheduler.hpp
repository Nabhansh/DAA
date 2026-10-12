#pragma once

#include "job.hpp"
#include <vector>

class DfsScheduler {
public:
    std::vector<int> topologicalSort(const std::vector<Job>& jobs);
};
