#include <iostream>
#include <string>

#include "cell.hpp"
#include "custom.hpp"
#include "geodesic.hpp"
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

static State parse_board(const YGame& game, const std::string& board_str) {

    State state{game};

    std::vector<Player> board{state.board.size(), Player::None};

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

enum class Game {
    Geodesic,
    Custom,
};

static Game parse_game(const std::string& game_str) {
    if (game_str == "geodesic") {
        return Game::Geodesic;
    } else if (game_str == "custom") {
        return Game::Custom;
    } else {
        throw std::runtime_error("error: invalid game type " + game_str);
    }
}

static void solve_game(const YGame& ygame, const std::string& board_str, const Player player, const bool moves) {

    State state = parse_board(ygame, board_str);

    std::cout << "Running alpha-beta for " << player << std::endl;

    if (moves) {
        const auto wins = winning_moves(state, ygame, player);

        std::cout << "Winning moves: ";
        for (const auto cell : wins) {
            std::cout << static_cast<uint32_t>(cell) << ' ';
        }
        std::cout << std::endl;
    } else {
        const auto outcome = winning_outcome(state, ygame, player);

        std::cout << "Outcome: " << outcome << std::endl;
    }
}

int main(const int argc, const char* argv[]) {

    try {
        // Defaults for each argument
        Cell base = 3;
        Player player = Player::Black;
        std::string board_str = "";
        bool moves = false;
        Game game = Game::Geodesic;
        std::string board_file = "sample-board.txt";

        for (int i = 1; i < argc; ++i) {

            const std::string arg{argv[i]};

            if (arg.rfind("--game=", 0) == 0) {
                game = parse_game(arg.substr(7));
            } else if (arg.rfind("--base=", 0) == 0) {
                base = parse_base(arg.substr(7));
            } else if (arg.rfind("--player=", 0) == 0) {
                player = parse_player(arg.substr(9));
            } else if (arg.rfind("--board=", 0) == 0) {
                board_str = arg.substr(8);
            } else if (arg.rfind("--board-file=", 0) == 0) {
                board_file = arg.substr(13);
            } else if (arg == "--moves") {
                moves = true;
            } else if (arg == "-h" || arg == "--help") {
                std::cout << "Usage: " << argv[0] << " [options]" << std::endl
                          << "--game={geodesic,custom}  The type of Y game to play (default: geodesic)" << std::endl
                          << "--board='B1 W3 B5'        The initial board state (default: empty)" << std::endl
                          << "--player={black,white}    The player to go first (default: black)" << std::endl
                          << "--moves                   Show all winning moves (default: show only a single winning move, if any)" << std::endl
                          << "--base=N                  The size of the base of the board (geodesic Y only, default: 3)" << std::endl
                          << "--board-file=<path>       Path to the board file (custom Y only, default: sample-board.txt)" << std::endl;
                return EXIT_SUCCESS;
            } else {
                throw std::runtime_error("unknown argument: " + arg);
            }
        }

        // TODO rethink how to do this...
        if (game == Game::Geodesic) {
            GeodesicY ygame{base};
            solve_game(ygame, board_str, player, moves);
        } else if (game == Game::Custom) {
            CustomY ygame{board_file};
            solve_game(ygame, board_str, player, moves);
        }

    } catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
    }
}
