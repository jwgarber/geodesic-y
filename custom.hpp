#pragma once

#include <string>

#include "ygame.hpp"

class CustomY : public YGame {
    private:
    std::vector<std::vector<Cell>> graph_;
    std::vector<std::vector<Cell>> perms_;
    std::vector<Edge> edges_;

    public:
    explicit CustomY(const std::string& file_path);

    const std::vector<std::vector<Cell>>& graph() const override {
        return graph_;
    }

    const std::vector<std::vector<Cell>>& perms() const override {
        return perms_;
    }

    Edge cell_edge(Cell cell) const override {
        return edges_.at(cell);
    }
};
