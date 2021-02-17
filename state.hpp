#pragma once

#include <vector>

#include "cell.hpp"
#include "ygame.hpp"

struct Node {
    Player player;
    Cell parent;
    Cell size;
    Edge edge;

    explicit Node() {}

    explicit Node(const Player player_, const Cell parent_, const Cell size_, const Edge edge_)
        : player{player_}, parent{parent_}, size{size_}, edge{edge_} {}
};

struct State {
    std::vector<Node> board;

    explicit State(const YGame& game);

    Cell root(const Cell cell);
    void join(const Cell a, const Cell b);
    void move(const YGame& game, const Player player, const Cell cell);
    bool won(const Cell cell);
};

