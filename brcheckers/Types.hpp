#pragma once
#include <cstdint>
#include "SimpleVector.hpp"

enum class Cell : int8_t {
    Empty = 0,
    WhiteMan,
    WhiteKing,
    BlackMan,
    BlackKing
};

enum class Player : int8_t {
    White = 0,
    Black = 1
};

enum class GameStatus : int8_t {
    InProgress,
    WhiteWon,
    BlackWon,
    Draw
};

struct Pos {
    int8_t r = -1;
    int8_t c = -1;

    constexpr bool operator==(const Pos& o) const { return r == o.r && c == o.c; }
    constexpr bool operator!=(const Pos& o) const { return !(*this == o); }
    constexpr bool is_valid() const { return r >= 0 && r < 8 && c >= 0 && c < 8; }
};

struct Move {
    Pos from;
    Pos to;
    SimpleVector<Pos> captured; // Координаты всех снятых шашек за этот ход
    SimpleVector<Pos> path;     // Клетки приземления между прыжками (для анимации)
    bool became_king = false;   // Флаг превращения в дамку в конце хода

    bool is_capture() const { return !captured.empty(); }
};