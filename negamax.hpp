#pragma once

#include <vector>

#include "cell.hpp"
#include "game.hpp"
#include "state.hpp"

Outcome winning_outcome(const State& state, const Game& game, const Player player);
std::vector<Cell> winning_moves(const State& state, const Game& game, const Player player);

