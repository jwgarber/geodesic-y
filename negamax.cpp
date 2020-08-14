#include "negamax.hpp"

#include <future>
#include <iostream>
#include <map>
#include <utility>

// TODO: we might be able to represent the board as an integer, which may make things easier
static std::vector<Player> min_perm(const std::vector<Player>& board, const Game& game) {

    auto min = board;

    std::vector<Player> newboard{board.size()};

    for (const auto& perm : game.perms) {

        for (Cell cell = 0; cell < board.size(); ++cell) {
            newboard.at(perm.at(cell)) = board.at(cell);
        }

        if (newboard < min) {
            min = newboard;
        }
    }

    return min;
}

static std::map<std::vector<Player>, Cell> unique_moves(const State& state, const Game& game, const Player player) {

    // Copy the current players into a vector
    std::vector<Player> board{state.board.size()};
    for (Cell cell = 0; cell < state.board.size(); ++cell) {
        board.at(cell) = state.board.at(cell).player;
    }

    std::map<std::vector<Player>, Cell> moves{};
    for (Cell cell = 0; cell < state.board.size(); ++cell) {
        if (state.board.at(cell).player == Player::None) {
            board.at(cell) = player;

            auto min = min_perm(board, game);
            moves[min] = cell;

            board.at(cell) = Player::None;
        }
    }

    return moves;
}

static Outcome negamax(const State& state, const Game& game, const Player player) {

    // Undoing moves is tricky because of union-find, so just create a copy
    // of the state for the child.
    State child = state;

    const auto moves = unique_moves(state, game, player);
    for (const auto& p : moves) {
        const auto cell = p.second;

        child = state;
        child.move(game, player, cell);

        // Normally we check if the game is finished at the start of this function
        // but this is more efficient since we can check immediately if the game is over
        if (child.won(cell)) {
            return Outcome::Win;
        }

        const auto value = negamax(child, game, !player);

        // If this is a losing position for the other player, then we won.
        if (value == Outcome::Lose) {
            return Outcome::Win;
        }
    }

    return Outcome::Lose;
}

Outcome winning_outcome(const State& state, const Game& game, const Player player) {

    // Undoing moves is tricky because of union-find, so just create a copy
    // of the state for the child.
    State child = state;

    const auto moves = unique_moves(state, game, player);
    for (const auto& p : moves) {
        const auto cell = p.second;

        std::cout << "Analyzing move " << static_cast<uint32_t>(cell) << ": " << std::flush;

        child = state;
        child.move(game, player, cell);

        Outcome outcome;

        // Normally we check if the game is finished at the start of this function
        // but this is more efficient since we can check immediately if the game is over
        if (child.won(cell)) {
            outcome = Outcome::Win;
        } else {
            outcome = -negamax(child, game, !player);
        }

        std::cout << outcome << std::endl;

        // Short-circuit if a winning move is found
        if (outcome == Outcome::Win) {
            return Outcome::Win;
        }
    }

    return Outcome::Lose;
}

std::vector<Cell> winning_moves(const State& state, const Game& game, const Player player) {

    const auto lambda = [&](const Cell cell) {

        State child = state;
        child.move(game, player, cell);

        // Normally we check if the game is finished at the start of this function
        // but this is more efficient since we can check immediately if the game is over
        if (child.won(cell)) {
            return Outcome::Win;
        }

        const auto outcome = -negamax(child, game, !player);

        return outcome;
    };

    std::vector<std::pair<Cell, std::future<Outcome>>> futures{};

    std::cout << "Analyzing moves ";
    for (Cell cell = 0; cell < state.board.size(); ++cell) {
        if (state.board.at(cell).player == Player::None) {
            std::cout << static_cast<uint32_t>(cell) << ' ';
            futures.emplace_back(cell, std::async(std::launch::async, lambda, cell));
        }
    }
    std::cout << std::endl;

    std::vector<Cell> wins{};
    for (auto& fut : futures) {
        const auto cell = fut.first;
        const auto outcome = fut.second.get();
        std::cout << "Move " << static_cast<uint32_t>(cell) << ": " << outcome << std::endl;
        if (outcome == Outcome::Win) {
            wins.push_back(cell);
        }
    }

    return wins;
}
