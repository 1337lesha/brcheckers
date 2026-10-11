#include "Evaluator.hpp"

int Evaluator::get_advancement_bonus(int r, Player p) {
    // Чем ближе к последней горизонтали, тем ценнее позиция
    return (p == Player::White) ? (7 - r) * 5 : r * 5;
}

int Evaluator::get_center_bonus(int r, int c) {
    if ((r >= 2 && r <= 5) && (c >= 2 && c <= 5)) {
        return 8;
    }
    return 0;
}

int Evaluator::evaluate(const Board& board) {
    int white_score = 0;
    int black_score = 0;

    int white_pieces = 0;
    int black_pieces = 0;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Pos p{ static_cast<int8_t>(r), static_cast<int8_t>(c) };
            Cell cell = board.get(p);

            if (cell == Cell::Empty) continue;

            if (Board::is_white(cell)) {
                white_pieces++;
                if (cell == Cell::WhiteKing) {
                    white_score += KING_WEIGHT;
                }
                else {
                    white_score += MAN_WEIGHT;
                    white_score += get_advancement_bonus(r, Player::White);
                    white_score += get_center_bonus(r, c);
                }
                // Защита тыла: бонус за шашки на базовой горизонтали
                if (r == 7) white_score += 15;
            }
            else {
                black_pieces++;
                if (cell == Cell::BlackKing) {
                    black_score += KING_WEIGHT;
                }
                else {
                    black_score += MAN_WEIGHT;
                    black_score += get_advancement_bonus(r, Player::Black);
                    black_score += get_center_bonus(r, c);
                }
                if (r == 0) black_score += 15;
            }
        }
    }

    if (white_pieces == 0) return LOSS_SCORE;
    if (black_pieces == 0) return WIN_SCORE;

    return white_score - black_score;
}