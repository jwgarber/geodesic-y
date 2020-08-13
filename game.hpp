#pragma once

#include <vector>

#include "cell.hpp"

struct Game {

    std::vector<std::vector<Cell>> graph;
    std::vector<std::vector<Cell>> perms;

    explicit Game(const Cell base);
};
