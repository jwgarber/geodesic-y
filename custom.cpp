#include <sstream>

#include "custom.hpp"
#include "util.hpp"

static Cell parse_num_board_cells(const std::vector<std::string>& lines) {

    for (const auto& line : lines) {
        if (line.rfind("num_board_cells:", 0) == 0) {
            const auto comps = split(line, ':');
            if (comps.size() != 2) {
                std::ostringstream err{};
                err << "error: unable to parse \"" << line << "\"";
                throw std::runtime_error(err.str());
            }

            const auto num_board_cells = parse_int<Cell>(trim_copy(comps.at(1)));

            if (num_board_cells == 0) {
                throw std::runtime_error("error: num_board_cells cannot be 0");
            }

            return num_board_cells;
        }
    }

    throw std::runtime_error("error: no value for num_board_cells found");
}

static std::vector<Cell> parse_cell_list(const std::string& line, const Cell num_board_cells) {
    const auto comps = split(line, ' ');

    std::vector<Cell> list{};

    for (const auto& comp : comps) {
        const auto num = parse_int<Cell>(comp);

        if (num >= num_board_cells) {
            std::ostringstream err{};
            err << "error: cell " << static_cast<uint32_t>(num) << " is greater than the board size";
            throw std::runtime_error(err.str());
        }

        list.push_back(num);
    }

    return list;
}

static std::vector<std::vector<Cell>> parse_board_graph(const std::vector<std::string>& lines, const Cell num_board_cells) {

    std::vector<std::vector<Cell>> board_graph{num_board_cells};

    for (const auto& line : lines) {
        if (line.rfind("-", 0) == 0) {
            const auto new_line = replace_copy(line, "-", "");
            const auto comps = split(new_line, ':');

            if (comps.size() != 2) {
                std::ostringstream err{};
                err << "error: unable to parse \"" << line << "\"";
                throw std::runtime_error(err.str());
            }

            const auto cell_number = parse_int<Cell>(trim_copy(comps.at(0)));

            if (cell_number >= num_board_cells) {
                std::ostringstream err{};
                err << "error: cell " << static_cast<uint32_t>(cell_number) << " is greater than the board size";
                throw std::runtime_error(err.str());
            }

            const auto neighbors = parse_cell_list(comps.at(1), num_board_cells);

            if (neighbors.empty()) {
                std::ostringstream err{};
                err << "error: cell " << static_cast<uint32_t>(cell_number) << " does not have any neighbors";
                throw std::runtime_error(err.str());
            }

            if (!board_graph.at(cell_number).empty()) {
                std::ostringstream err{};
                err << "error: neighbors for cell " << static_cast<uint32_t>(cell_number) << " specified multiple times";
                throw std::runtime_error(err.str());
            }

            board_graph.at(cell_number) = neighbors;
        }
    }

    for (Cell i = 0; i < num_board_cells; ++i) {
        if (board_graph.at(i).empty()) {
            std::ostringstream err{};
            err << "error: no neighbors given for cell " << static_cast<uint32_t>(i);
            throw std::runtime_error(err.str());
        }
    }

    return board_graph;
}

static std::vector<Cell> parse_edge_list(const std::string& line, const Cell num_board_cells) {

    const auto comps = split(line, ':');
    if (comps.size() != 2) {
        std::ostringstream err{};
        err << "error: unable to parse \"" << line << "\"";
        throw std::runtime_error(err.str());
    }

    return parse_cell_list(comps.at(1), num_board_cells);
}

static std::vector<Edge> parse_cell_edges(const std::vector<std::string>& lines, const Cell num_board_cells) {

    std::vector<Edge> edges{num_board_cells, Edge::None};

    for (const auto& line : lines) {

        Edge edge = Edge::None;
        std::vector<Cell> cells{};

        if (line.rfind("left_edge_cells:", 0) == 0) {
            edge = Edge::Left;
            cells = parse_edge_list(line, num_board_cells);
        }

        if (line.rfind("bottom_edge_cells:", 0) == 0) {
            edge = Edge::Bottom;
            cells = parse_edge_list(line, num_board_cells);
        }

        if (line.rfind("right_edge_cells:", 0) == 0) {
            edge = Edge::Right;
            cells = parse_edge_list(line, num_board_cells);
        }

        for (const auto cell : cells) {
            edges.at(cell) |= edge;
        }
    }

    return edges;
}

CustomY::CustomY(const std::string& file_path) {

    const auto file = read_file(file_path);

    auto lines = split(file, '\n');
    for (auto& line : lines) {
        line = trim_copy(line);
    }

    const auto num_board_cells = parse_num_board_cells(lines);

    graph_ = parse_board_graph(lines, num_board_cells);
    edges_ = parse_cell_edges(lines, num_board_cells);
}
