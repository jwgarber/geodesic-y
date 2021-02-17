#include "state.hpp"

State::State(const YGame& game) {

    board.resize(game.graph().size());

    for (Cell cell = 0; cell < board.size(); ++cell) {
        board.at(cell) = Node(Player::None, cell, 1, game.cell_edge(cell));
    }
}

Cell State::root(const Cell cell) {

    auto parent = board.at(cell).parent;

    if (parent != cell) {
        // Follow the parent chain to the root
        do {
            parent = board.at(parent).parent;
        } while (parent != board.at(parent).parent);
        // Do path compression, but only on the current one to avoid recursion
        board.at(cell).parent = parent;
    }

    return parent;
}

void State::join(const Cell a, const Cell b) {
    auto a_root = root(a);
    auto b_root = root(b);

    if (a_root == b_root) {
        // Already in the same group
        return;
    }

    if (board.at(a_root).size < board.at(b_root).size) {
        // Make group a have the larger tree
        std::swap(a_root, b_root);
    }

    // Join group b to group a
    board.at(b_root).parent = a_root;
    board.at(a_root).size += board.at(b_root).size;
    board.at(a_root).edge |= board.at(b_root).edge;
}

void State::move(const YGame& game, const Player player, const Cell cell) {

    board.at(cell).player = player;

    for (const auto nhbr : game.graph().at(cell)) {
        if (board.at(nhbr).player == player) {
            join(cell, nhbr);
        }
    }
}

bool State::won(const Cell cell) {
    return board.at(root(cell)).edge == Edge::All;
}
