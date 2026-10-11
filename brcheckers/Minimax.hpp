#pragma once
#include <optional>
#include "SimpleVector.hpp"
#include "Board.hpp"
#include "MoveGenerator.hpp"
#include "Types.hpp"
#include "Evaluator.hpp"

class Minimax {
public:
    // Поиск оптимального хода с заданной глубиной полуходов
    static std::optional<Move> find_best_move(Board& board, int depth);

private:
    static int search(Board& board, int depth, int alpha, int beta, bool is_maximizing);
};