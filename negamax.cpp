#include "negamax.hpp"

#include <iostream>

static Outcome negamax(const State& state, const Game& game, const Player player) {

    // Undoing moves is tricky because of union-find, so just create a copy
    // of the state for the child.
    State child = state;
    for (Cell cell = 0; cell < state.board.size(); ++cell) {

        if (state.board.at(cell).player == Player::None) {

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
    }

    return Outcome::Lose;
}

std::vector<Cell> winning_moves(const State& state, const Game& game, const Player player) {

    std::vector<Cell> wins{};

    // Undoing moves is tricky because of union-find, so just create a copy
    // of the state for the child.
    State child = state;
    for (Cell cell = 0; cell < state.board.size(); ++cell) {
        if (state.board.at(cell).player == Player::None) {
            std::cout << "Analyzing move " << static_cast<uint32_t>(cell) << ": " << std::flush;

            child = state;
            child.move(game, player, cell);

            Outcome outcome = Outcome::Lose;

            // Normally we check if the game is finished at the start of this function
            // but this is more efficient since we can check immediately if the game is over
            if (child.won(cell)) {
                outcome = Outcome::Win;
            } else {
                const auto value = negamax(child, game, !player);

                // If this is a losing position for the other player, then we won.
                if (value == Outcome::Lose) {
                    outcome = Outcome::Win;
                }
            }

            std::cout << outcome << std::endl;

            if (outcome == Outcome::Win) {
                wins.push_back(cell);
            }
        }
    }

    return wins;
}
