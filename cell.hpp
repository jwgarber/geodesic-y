#pragma once

#include <ostream>

using Cell = uint8_t;

enum class Player : uint8_t {
    Black,
    White,
    None,
};

static inline Player operator!(const Player player) {
    switch (player) {
        case Player::Black: return Player::White;
        case Player::White: return Player::Black;
        case Player::None: return Player::None;
    }
}

static inline std::ostream& operator<<(std::ostream& os, const Player player) {
    switch (player) {
        case Player::Black: return os << "black";
        case Player::White: return os << "white";
        case Player::None: return os << "none";
    }
}

enum class Outcome : int32_t {
    Win,
    Lose,
};

static inline std::ostream& operator<<(std::ostream& os, const Outcome outcome) {
    switch (outcome) {
        case Outcome::Win: return os << "win";
        case Outcome::Lose: return os << "lose";
    }
}

static inline Outcome operator-(const Outcome outcome) {
    switch (outcome) {
        case Outcome::Win: return Outcome::Lose;
        case Outcome::Lose: return Outcome::Win;
    }
}

enum class Edge : uint8_t {
    None = 0x0,
    Right = 0x1,
    Bottom = 0x2,
    Left = 0x4,
    All = 0x7,
};

static inline void operator|=(Edge& lhs, const Edge rhs) {
    const uint8_t val = static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs);
    lhs = static_cast<Edge>(val);
}

static inline Cell board_size(const Cell base) {
  return 3 * base * (base - 1) / 2;
}

static inline Cell top_cell(const Cell base) {
  // The smallest node of the outer ring is equal to the size of the
  // base - 1 board. This even works when the base size is 2, where
  // the inner ring then has size 0.
  return board_size(base - 1);
}

static inline Cell right_cell(const Cell base) {
  return top_cell(base) + base - 1;
}

static inline Cell left_cell(const Cell base) {
  return right_cell(base) + base - 1;
}

