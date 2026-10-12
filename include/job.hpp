#pragma once

#include <string>
#include <vector>

struct Job {
    int id;
    std::string tile_name;
    int processing_cost;
    std::vector<int> dependencies;
};