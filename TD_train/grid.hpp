#pragma once

#include "nlohmann/json.hpp"
#include <fstream>
using json = nlohmann::json;

struct Position
{
    int x {0};
    int y {0};

    // This macro generates the to_json and from_json functions for the Position struct
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y)
};

struct GridConfig
{
    int size_grid {0};
    Position origin {};
    std::vector<Position> path {};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GridConfig, size_grid, origin, path)
};