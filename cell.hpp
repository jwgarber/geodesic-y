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

static inline std::ostream& operator<<(std::ostream& os, const Edge edge) {
    switch (static_cast<uint8_t>(edge)) {
        case 0: return os << "none";
        case 1: return os << "right";
        case 2: return os << "bottom";
        case 3: return os << "bottom+right";
        case 4: return os << "left";
        case 5: return os << "left+right";
        case 6: return os << "left+bottom";
        case 7: return os << "all";
    }
}
