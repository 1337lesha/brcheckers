#include "Minimax.hpp"

int Minimax::search(Board& board, int depth, int alpha, int beta, bool is_maximizing) {
    SimpleVector<Move> legal_moves = MoveGenerator::generate_legal_moves(board);

    // Терминальный узел: победа соперника при отсутствии ходов
    if (legal_moves.empty()) {
        return is_maximizing ? Evaluator::LOSS_SCORE - depth : Evaluator::WIN_SCORE + depth;
    }

    if (depth == 0) {
        return Evaluator::evaluate(board);
    }

    if (is_maximizing) {
        int max_eval = -200000;
        for (size_t i = 0; i < legal_moves.size(); ++i) {
            Board temp_board = board;
            temp_board.apply_move(legal_moves[i]);

            int eval = search(temp_board, depth - 1, alpha, beta, false);
            if (eval > max_eval) max_eval = eval;
            if (eval > alpha) alpha = eval;
            if (beta <= alpha) break; // Beta-отсечение
        }
        return max_eval;
    }
    else {
        int min_eval = 200000;
        for (size_t i = 0; i < legal_moves.size(); ++i) {
            Board temp_board = board;
            temp_board.apply_move(legal_moves[i]);

            int eval = search(temp_board, depth - 1, alpha, beta, true);
            if (eval < min_eval) min_eval = eval;
            if (eval < beta) beta = eval;
            if (beta <= alpha) break; // Alpha-отсечение
        }
        return min_eval;
    }
}

std::optional<Move> Minimax::find_best_move(Board& board, int depth) {
    SimpleVector<Move> legal_moves = MoveGenerator::generate_legal_moves(board);
    if (legal_moves.empty()) return std::nullopt;

    Move best_move = legal_moves[0];
    bool is_white = (board.turn() == Player::White);

    if (is_white) {
        int best_score = -200000;
        int alpha = -200000;
        int beta = 200000;

        for (size_t i = 0; i < legal_moves.size(); ++i) {
            Board temp_board = board;
            temp_board.apply_move(legal_moves[i]);

            int score = search(temp_board, depth - 1, alpha, beta, false);
            if (score > best_score) {
                best_score = score;
                best_move = legal_moves[i];
            }
            if (score > alpha) alpha = score;
        }
    }
    else {
        int best_score = 200000;
        int alpha = -200000;
        int beta = 200000;

        for (size_t i = 0; i < legal_moves.size(); ++i) {
            Board temp_board = board;
            temp_board.apply_move(legal_moves[i]);

            int score = search(temp_board, depth - 1, alpha, beta, true);
            if (score < best_score) {
                best_score = score;
                best_move = legal_moves[i];
            }
            if (score < beta) beta = score;
        }
    }

    return best_move;
}