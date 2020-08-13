#include "game.hpp"

#include <algorithm>
#include <stdexcept>

// The adjacency graph for the board
static std::vector<std::vector<Cell>> gen_graph(const Cell base) {

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

    auto symmetric = neighbors;

    // Make the graph symmetric
    for (Cell i = 0; i < neighbors.size(); ++i) {
        for (const auto j : neighbors.at(i)) {
            symmetric.at(j).push_back(i);
        }
    }

    for (auto& vec : symmetric) {
        std::sort(std::begin(vec), std::end(vec));
    }

    return symmetric;
}

// Rotate 1/3 the way around CW
static std::vector<Cell> rotate(const std::vector<Cell>& board, const Cell base) {

    std::vector<Cell> rotated = board;

    // right -> bottom
    // bottom -> left
    // left -> right
    for (Cell ring = 2; ring <= base; ++ring) {

        const auto top = top_cell(ring);
        const auto right = right_cell(ring);
        const auto left = left_cell(ring);

        // The number of cells in a single side of the ring
        const Cell side = ring - 1;

        for (Cell i = 0; i < side; ++i) {
            rotated.at(right + i) = board.at(top + i);
            rotated.at(left + i) = board.at(right + i);
            rotated.at(top + i) = board.at(left + i);
        }
    }

    return rotated;
}

// Reflect the board along the line of symmetry passing through the top node
static std::vector<Cell> reflect(const std::vector<Cell>& board, const Cell base) {

    std::vector<Cell> reflected = board;

    for (Cell ring = 2; ring <= base; ++ring) {

        const auto right = right_cell(ring);
        const auto left = left_cell(ring);

        // The number of cells in a single side of the ring
        const Cell side = ring - 1;

        // Swap the left and right sides
        for (Cell i = 0; i < side; ++i) {
            reflected.at(left + i) = board.at(right - i);
            reflected.at(right - i) = board.at(left + i);
        }

        // Half the number of cells on the bottom, excluding left and right, rounded down
        const Cell half = (side - 1) / 2;

        // Now reflect the rest of the bottom side in itself
        // We start at 1 here to prevent swapping the left and right nodes again
        for (Cell i = 1; i <= half; ++i) {
            reflected.at(left - i) = board.at(right + i);
            reflected.at(right + i) = board.at(left - i);
        }
    }

    return reflected;
}

// Generate the non-trivial permutations of the board
static std::vector<std::vector<Cell>> gen_perms(const Cell base) {

    std::vector<Cell> id{};
    id.resize(board_size(base));

    for (Cell i = 0; i < id.size(); ++i) {
        id.at(i) = i;
    }

    const auto rot = rotate(id, base);
    const auto rotrot = rotate(rot, base);

    const auto ref = reflect(id, base);
    const auto rotref = rotate(ref, base);
    const auto rotrotref = rotate(rotref, base);

    return {rot, rotrot, ref, rotref, rotrotref};
}

Game::Game(const Cell base) {
    graph = gen_graph(base);
    perms = gen_perms(base);
}
