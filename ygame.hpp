#pragma once

#include <vector>

#include "cell.hpp"

struct YGame {
    virtual const std::vector<std::vector<Cell>>& graph() const = 0;
    virtual const std::vector<std::vector<Cell>>& perms() const = 0;
    virtual Edge cell_edge(Cell cell) const = 0;
};
