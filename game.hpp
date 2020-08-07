#pragma once

#include <vector>

#include "cell.hpp"

struct Game {

    std::vector<std::vector<Cell>> graph;

    explicit Game(const Cell base);
};
