#include "game.hpp"

#include <algorithm>
#include <stdexcept>

Game::Game(const Cell base) {

    std::vector<std::vector<Cell>> neighbors{board_size(base)};

    neighbors.at(0).push_back(1);
    neighbors.at(1).push_back(2);
    neighbors.at(2).push_back(0);

    // This loop will be skipped if base == 2
    for (Cell ring = 3; ring <= base; ++ring) {

        const auto top = top_cell(ring);
        const auto right = right_cell(ring);
        const auto left = left_cell(ring);

        const auto ring_size = board_size(ring);
        const Cell last = ring_size - 1;

        for (Cell cell = top; cell < ring_size; ++cell) {

            // Add the next node clockwise in the ring
            if (cell == last) {
                neighbors.at(cell).push_back(top);
            } else {
                neighbors.at(cell).push_back(cell + 1);
            }

            // Now add the cells in the layer below
            const auto top_below = top_cell(ring - 1);
            const auto right_below = right_cell(ring - 1);
            const auto left_below = left_cell(ring - 1);

            if (cell == top) {
                neighbors.at(cell).push_back(top_below);
            } else if ((top < cell) && (cell < right)) {
                const Cell nhbr = top_below + cell - top;
                neighbors.at(cell).push_back(nhbr - 1);
                neighbors.at(cell).push_back(nhbr);
            } else if (cell == right) {
                neighbors.at(cell).push_back(right_below);
            } else if ((right < cell) && (cell < left)) {
                const Cell nhbr = right_below + cell - right;
                neighbors.at(cell).push_back(nhbr - 1);
                neighbors.at(cell).push_back(nhbr);
            } else if (cell == left) {
                neighbors.at(cell).push_back(left_below);
            } else if (cell < last) {
                const Cell nhbr = left_below + cell - left;
                neighbors.at(cell).push_back(nhbr - 1);
                neighbors.at(cell).push_back(nhbr);
            } else if (cell == last) {
                const Cell nhbr = left_below + cell - left;
                neighbors.at(cell).push_back(nhbr - 1);
                neighbors.at(cell).push_back(top_below);
            } else {
                throw std::runtime_error("unknown cell: " + std::to_string(cell));
            }
        }
    }

    graph = neighbors;

    // Make the graph symmetric
    for (Cell i = 0; i < neighbors.size(); ++i) {
        for (const auto j : neighbors.at(i)) {
            graph.at(j).push_back(i);
        }
    }

    for (auto& vec : graph) {
        std::sort(std::begin(vec), std::end(vec));
    }
}
