#include <iostream>
#include <string>

#include "cell.hpp"
#include "negamax.hpp"
#include "state.hpp"
#include "util.hpp"

static Cell parse_base(const std::string& base_str) {

    auto base = parse_int<Cell>(base_str);

    if (base < 2) {
        throw std::runtime_error("invalid base: " + base_str);
    }

    // Cells are represented as uint8_t, so the largest base
    // we can use right now is 13.
    if (base > 13) {
        throw std::runtime_error("base too large: " + base_str);
    }

    return base;
}

static Player parse_player(const std::string& player_str) {

    if (player_str == "black") {
        return Player::Black;
    } else if (player_str == "white") {
        return Player::White;
    } else {
        throw std::runtime_error("invalid player: " + player_str);
    }
}

static State parse_board(const Cell base, const Game& game, const std::string& board_str) {

    State state{base};

    std::vector<Player> board{board_size(base), Player::None};

    const auto moves = split(board_str, ' ');

    for (const auto& move : moves) {

        // The split function skips any empty strings
        const auto player_chr = move.at(0);

        Player player;
        if (player_chr == 'B') {
            player = Player::Black;
        } else if (player_chr == 'W') {
            player = Player::White;
        } else {
            throw std::runtime_error("invalid player: " + std::string{1, player_chr});
        }

        const auto cell = parse_int<Cell>(move.substr(1));

        if (cell >= state.board.size()) {
            throw std::runtime_error("invalid position: " + std::to_string(cell));
        }

        if (board.at(cell) == !player) {
            throw std::runtime_error("error: conflicting players for cell " + std::to_string(cell));
        }

        board.at(cell) = player;
    }

    for (Cell cell = 0; cell < board.size(); ++cell) {
        const auto player = board.at(cell);
        if (player != Player::None) {
            state.move(game, player, cell);

            if (state.won(cell)) {
                throw std::runtime_error("error: initial board cannot be won");
            }
        }
    }

    return state;
}

int main(const int argc, const char* argv[]) {

    try {
        // Defaults for each argument
        Cell base = 3;
        Player player = Player::Black;
        std::string board_str = "";
        bool moves = false;

        for (int i = 1; i < argc; ++i) {

            const std::string arg{argv[i]};

            if (arg.rfind("--base=", 0) == 0) {
                base = parse_base(arg.substr(7));
            } else if (arg.rfind("--player=", 0) == 0) {
                player = parse_player(arg.substr(9));
            } else if (arg.rfind("--board=", 0) == 0) {
                board_str = arg.substr(8);
            } else if (arg == "--moves") {
                moves = true;
            } else {
                throw std::runtime_error("unknown argument: " + arg);
            }
        }

        const Game game{base};

        State state = parse_board(base, game, board_str);

        std::cout << "Running alpha-beta for " << player
                  << " on base " << static_cast<uint32_t>(base) << " board"
                  << std::endl;

        if (moves) {
            const auto wins = winning_moves(state, game, player);

            std::cout << "Winning moves: ";
            for (const auto cell : wins) {
                std::cout << static_cast<uint32_t>(cell) << ' ';
            }
            std::cout << std::endl;
        } else {
            const auto outcome = winning_outcome(state, game, player);

            std::cout << "Outcome: " << outcome << std::endl;
        }


    } catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
    }
}
