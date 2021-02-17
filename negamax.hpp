#pragma once

#include <vector>

#include "cell.hpp"
#include "ygame.hpp"
#include "state.hpp"

Outcome winning_outcome(const State& state, const YGame& game, const Player player);
std::vector<Cell> winning_moves(const State& state, const YGame& game, const Player player);

