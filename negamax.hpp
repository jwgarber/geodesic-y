#pragma once

#include <vector>

#include "cell.hpp"
#include "game.hpp"
#include "state.hpp"

std::vector<Cell> winning_moves(const State& state, const Game& game, const Player player);
