#pragma once

#include "job.hpp"
#include <vector>
#include <stdexcept>

class Scheduler {
public:
    std::vector<int> topologicalSort(const std::vector<Job>& jobs);
};