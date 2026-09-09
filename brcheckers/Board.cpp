#include "Board.hpp"

Board::Board() {
    reset();
}

void Board::reset() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            grid_[r][c] = Cell::Empty;
            // В шашках задействованы только темные поля ((r + c) % 2 != 0)
            if ((r + c) % 2 != 0) {
                if (r < 3)      grid_[r][c] = Cell::BlackMan;
                else if (r > 4) grid_[r][c] = Cell::WhiteMan;
            }
        }
    }
    turn_ = Player::White;
}

Cell Board::get(Pos p) const {
    if (!p.is_valid()) return Cell::Empty;
    return grid_[p.r][p.c];
}

Cell Board::get(int r, int c) const {
    return get(Pos{ static_cast<int8_t>(r), static_cast<int8_t>(c) });
}

void Board::set(Pos p, Cell piece) {
    if (p.is_valid()) {
        grid_[p.r][p.c] = piece;
    }
}

bool Board::is_white(Cell p) { return p == Cell::WhiteMan || p == Cell::WhiteKing; }
bool Board::is_black(Cell p) { return p == Cell::BlackMan || p == Cell::BlackKing; }
bool Board::is_king(Cell p) { return p == Cell::WhiteKing || p == Cell::BlackKing; }

bool Board::is_friendly(Cell piece, Player p) {
    if (piece == Cell::Empty) return false;
    return p == Player::White ? is_white(piece) : is_black(piece);
}

bool Board::is_enemy(Cell piece, Player p) {
    if (piece == Cell::Empty) return false;
    return p == Player::White ? is_black(piece) : is_white(piece);
}

void Board::apply_move(const Move& m) {
    Cell piece = get(m.from);
    set(m.from, Cell::Empty);

    // В бразильских шашках сбитые фигуры удаляются только по завершении всего хода
    for (size_t i = 0; i < m.captured.size(); ++i) {
        set(m.captured[i], Cell::Empty);
    }

    // Проверка превращения на последней горизонтали
    if (m.became_king) {
        piece = (turn_ == Player::White) ? Cell::WhiteKing : Cell::BlackKing;
    }

    set(m.to, piece);
    switch_turn();
}