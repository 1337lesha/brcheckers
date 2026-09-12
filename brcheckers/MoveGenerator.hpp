#pragma once
#include "SimpleVector.hpp"
#include "Board.hpp"
#include "Types.hpp"

class MoveGenerator {
public:
    // Генерация всех легальных ходов для текущего игрока с учетом взятий и большинства
    static SimpleVector<Move> generate_legal_moves(const Board& board);

    // Генерация легальных ходов для конкретного игрока
    static SimpleVector<Move> generate_legal_moves(const Board& board, Player player);

    // Проверка наличия доступных ходов у игрока (для детекции победы/поражения)
    static bool has_legal_moves(const Board& board, Player player);

    // Получение ходов только для конкретной выбранной клетки (для подсветки в UI)
    static SimpleVector<Move> get_moves_for_piece(const Board& board, Pos from);

    // Проверка, является ли конкретный ход легальным
    static bool is_legal_move(const Board& board, const Move& move);

private:
    static bool is_already_captured(const SimpleVector<Pos>& captured, Pos p);
    static bool are_captured_equal(const SimpleVector<Pos>& a, const SimpleVector<Pos>& b);
    static bool is_duplicate(const SimpleVector<Move>& moves, const Move& m);

    // Рекурсивный поиск цепочек взятий простой шашкой
    static void generate_man_captures(
        Cell grid[8][8],
        Pos start_pos,
        Pos current_pos,
        Player player,
        SimpleVector<Pos>& captured,
        SimpleVector<Pos>& path,
        SimpleVector<Move>& moves
    );

    // Рекурсивный поиск цепочек взятий дальнобойной дамкой
    static void generate_king_captures(
        Cell grid[8][8],
        Pos start_pos,
        Pos current_pos,
        Player player,
        SimpleVector<Pos>& captured,
        SimpleVector<Pos>& path,
        SimpleVector<Move>& moves
    );

    // Генерация тихих (обычных) ходов, если взятия отсутствуют
    static void generate_quiet_moves(const Board& board, Player player, SimpleVector<Move>& moves);
};