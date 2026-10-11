#pragma once
#include "Board.hpp"
#include "Types.hpp"

class Evaluator {
public:
    static constexpr int WIN_SCORE = 100000;
    static constexpr int LOSS_SCORE = -100000;

    // Оценка состояния доски
    static int evaluate(const Board& board);

private:
    static constexpr int MAN_WEIGHT = 100;
    static constexpr int KING_WEIGHT = 350;

    // Бонус за продвижение простых шашек к дамочному полю
    static int get_advancement_bonus(int r, Player p);

    // Бонус за контроль центральных полей (c3, d4, e5, f6)
    static int get_center_bonus(int r, int c);
};