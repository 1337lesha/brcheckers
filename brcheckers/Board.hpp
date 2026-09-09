#pragma once
#include "Types.hpp"

class Board {
private:
    Cell grid_[8][8];
    Player turn_ = Player::White;

public:
    Board();

    void reset();
    Cell get(Pos p) const;
    Cell get(int r, int c) const;
    void set(Pos p, Cell piece);

    Player turn() const { return turn_; }
    void switch_turn() { turn_ = (turn_ == Player::White) ? Player::Black : Player::White; }

    void apply_move(const Move& m);

    static bool is_white(Cell piece);
    static bool is_black(Cell piece);
    static bool is_king(Cell piece);
    static bool is_friendly(Cell piece, Player p);
    static bool is_enemy(Cell piece, Player p);
};