#pragma once

#include "ygame.hpp"

class GeodesicY : public YGame {
    private:
    Cell base;
    std::vector<std::vector<Cell>> graph_;
    std::vector<std::vector<Cell>> perms_;

    public:
    explicit GeodesicY(const Cell base_);

    const std::vector<std::vector<Cell>>& graph() const override {
        return graph_;
    }

    const std::vector<std::vector<Cell>>& perms() const override {
        return perms_;
    }

    Edge cell_edge(Cell cell) const override;
};
