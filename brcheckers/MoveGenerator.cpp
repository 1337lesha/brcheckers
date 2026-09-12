#include "MoveGenerator.hpp"

bool MoveGenerator::is_already_captured(const SimpleVector<Pos>& captured, Pos p) {
    for (size_t i = 0; i < captured.size(); ++i) {
        if (captured[i] == p) return true;
    }
    return false;
}

bool MoveGenerator::are_captured_equal(const SimpleVector<Pos>& a, const SimpleVector<Pos>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        bool found = false;
        for (size_t j = 0; j < b.size(); ++j) {
            if (a[i] == b[j]) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

bool MoveGenerator::is_duplicate(const SimpleVector<Move>& moves, const Move& m) {
    for (size_t i = 0; i < moves.size(); ++i) {
        if (moves[i].from == m.from && moves[i].to == m.to) {
            if (are_captured_equal(moves[i].captured, m.captured)) {
                return true;
            }
        }
    }
    return false;
}

void MoveGenerator::generate_man_captures(
    Cell grid[8][8],
    Pos start_pos,
    Pos current_pos,
    Player player,
    SimpleVector<Pos>& captured,
    SimpleVector<Pos>& path,
    SimpleVector<Move>& moves
) {
    bool can_jump = false;
    // Простая шашка в бразильских шашках бьет во всех 4 направлениях
    constexpr int dr[4] = { -1, -1, 1, 1 };
    constexpr int dc[4] = { -1, 1, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        int mid_r = current_pos.r + dr[i];
        int mid_c = current_pos.c + dc[i];
        int to_r = current_pos.r + 2 * dr[i];
        int to_c = current_pos.c + 2 * dc[i];

        Pos mid_pos{ static_cast<int8_t>(mid_r), static_cast<int8_t>(mid_c) };
        Pos to_pos{ static_cast<int8_t>(to_r), static_cast<int8_t>(to_c) };

        if (!to_pos.is_valid()) continue;

        Cell mid_cell = grid[mid_r][mid_c];

        // нельзя рубить шашку, которая уже была сбита в этой цепочке
        if (Board::is_enemy(mid_cell, player) && !is_already_captured(captured, mid_pos)) {
            // Клетка приземления должна быть пустой
            if (grid[to_r][to_c] == Cell::Empty) {
                can_jump = true;

                Cell piece = grid[current_pos.r][current_pos.c];
                grid[current_pos.r][current_pos.c] = Cell::Empty;
                grid[to_r][to_c] = piece;
                captured.push_back(mid_pos);
                path.push_back(to_pos);

                // Рекурсивный поиск продолжения серии
                generate_man_captures(grid, start_pos, to_pos, player, captured, path, moves);

                // Backtracking
                path.pop_back();
                captured.pop_back();
                grid[to_r][to_c] = Cell::Empty;
                grid[current_pos.r][current_pos.c] = piece;
            }
        }
    }

    // Если продолжить бой нельзя, но взятие произошло — ветвь завершена
    if (!can_jump && !captured.empty()) {
        Move m;
        m.from = start_pos;
        m.to = current_pos;
        m.captured = captured;
        m.path = path;

        // Транзитное превращение: простая становится дамкой, только если ход
        // ОКОНЧАТЕЛЬНО завершился на последней горизонтали соперника
        if ((player == Player::White && current_pos.r == 0) ||
            (player == Player::Black && current_pos.r == 7)) {
            m.became_king = true;
        }
        else {
            m.became_king = false;
        }

        if (!is_duplicate(moves, m)) {
            moves.push_back(m);
        }
    }
}

void MoveGenerator::generate_king_captures(
    Cell grid[8][8],
    Pos start_pos,
    Pos current_pos,
    Player player,
    SimpleVector<Pos>& captured,
    SimpleVector<Pos>& path,
    SimpleVector<Move>& moves
) {
    bool found_any_jump = false;
    constexpr int dr[4] = { -1, -1, 1, 1 };
    constexpr int dc[4] = { -1, 1, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        int r = current_pos.r + dr[i];
        int c = current_pos.c + dc[i];
        Pos enemy_pos{ -1, -1 };

        //Сканируем диагональ в поисках первой фигуры на пути
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Cell cell = grid[r][c];
            if (cell != Cell::Empty) {
                // Если встретили вражескую шашку, которую еще не рубили в этом ходе
                if (Board::is_enemy(cell, player) &&
                    !is_already_captured(captured, { static_cast<int8_t>(r), static_cast<int8_t>(c) })) {
                    enemy_pos = { static_cast<int8_t>(r), static_cast<int8_t>(c) };
                }
                // Любая фигура (своя, вражеская или уже сбитая) блокирует дальнейший луч
                break;
            }
            r += dr[i];
            c += dc[i];
        }

        // Если враг найден, дамка может приземлиться на любую свободную клетку за ним
        if (enemy_pos.is_valid()) {
            int lr = enemy_pos.r + dr[i];
            int lc = enemy_pos.c + dc[i];

            while (lr >= 0 && lr < 8 && lc >= 0 && lc < 8) {
                if (grid[lr][lc] != Cell::Empty) {
                    // Препятствие за сбитой фигурой — дальше приземляться нельзя
                    break;
                }

                found_any_jump = true;
                Pos landing_pos{ static_cast<int8_t>(lr), static_cast<int8_t>(lc) };

                Cell king_piece = grid[current_pos.r][current_pos.c];
                grid[current_pos.r][current_pos.c] = Cell::Empty;
                grid[lr][lc] = king_piece;
                captured.push_back(enemy_pos);
                path.push_back(landing_pos);

                generate_king_captures(grid, start_pos, landing_pos, player, captured, path, moves);

                // Backtracking
                path.pop_back();
                captured.pop_back();
                grid[lr][lc] = Cell::Empty;
                grid[current_pos.r][current_pos.c] = king_piece;

                lr += dr[i];
                lc += dc[i];
            }
        }
    }

    // Если из данной позиции дамка не может продолжать бой, фиксируем ход
    if (!found_any_jump && !captured.empty()) {
        Move m;
        m.from = start_pos;
        m.to = current_pos;
        m.captured = captured;
        m.path = path;
        m.became_king = false;

        if (!is_duplicate(moves, m)) {
            moves.push_back(m);
        }
    }
}

void MoveGenerator::generate_quiet_moves(const Board& board, Player player, SimpleVector<Move>& moves) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Pos from{ static_cast<int8_t>(r), static_cast<int8_t>(c) };
            Cell piece = board.get(from);

            if (!Board::is_friendly(piece, player)) continue;

            if (Board::is_king(piece)) {
                // Тихие ходы дамки на любое число свободных клеток по 4 диагоналям
                constexpr int dr[4] = { -1, -1, 1, 1 };
                constexpr int dc[4] = { -1, 1, -1, 1 };

                for (int i = 0; i < 4; ++i) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];
                    while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                        Pos to{ static_cast<int8_t>(nr), static_cast<int8_t>(nc) };
                        if (board.get(to) != Cell::Empty) break;

                        Move m;
                        m.from = from;
                        m.to = to;
                        m.path.push_back(to);
                        m.became_king = false;
                        moves.push_back(m);

                        nr += dr[i];
                        nc += dc[i];
                    }
                }
            }
            else {
                // Тихие ходы простой шашки: только вперед на 1 клетку
                int forward_r = (player == Player::White) ? (r - 1) : (r + 1);
                constexpr int dc[2] = { -1, 1 };

                for (int i = 0; i < 2; ++i) {
                    int nc = c + dc[i];
                    Pos to{ static_cast<int8_t>(forward_r), static_cast<int8_t>(nc) };
                    if (to.is_valid() && board.get(to) == Cell::Empty) {
                        Move m;
                        m.from = from;
                        m.to = to;
                        m.path.push_back(to);
                        if ((player == Player::White && forward_r == 0) ||
                            (player == Player::Black && forward_r == 7)) {
                            m.became_king = true;
                        }
                        moves.push_back(m);
                    }
                }
            }
        }
    }
}

SimpleVector<Move> MoveGenerator::generate_legal_moves(const Board& board, Player player) {
    // Копируем сетку во временный буфер для безопасного DFS с backtracking
    Cell grid[8][8];
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            grid[r][c] = board.get(r, c);
        }
    }

    SimpleVector<Move> all_captures;

    // Поиск всех возможных взятий по всей доске
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Pos pos{ static_cast<int8_t>(r), static_cast<int8_t>(c) };
            Cell piece = grid[r][c];

            if (!Board::is_friendly(piece, player)) continue;

            SimpleVector<Pos> captured;
            SimpleVector<Pos> path;

            if (Board::is_king(piece)) {
                generate_king_captures(grid, pos, pos, player, captured, path, all_captures);
            }
            else {
                generate_man_captures(grid, pos, pos, player, captured, path, all_captures);
            }
        }
    }

    // Если есть взятия, применяем правило большинства
    if (!all_captures.empty()) {
        size_t max_captures = 0;
        for (size_t i = 0; i < all_captures.size(); ++i) {
            if (all_captures[i].captured.size() > max_captures) {
                max_captures = all_captures[i].captured.size();
            }
        }

        SimpleVector<Move> filtered_captures;
        for (size_t i = 0; i < all_captures.size(); ++i) {
            if (all_captures[i].captured.size() == max_captures) {
                filtered_captures.push_back(all_captures[i]);
            }
        }
        return filtered_captures;
    }

    // Если взятий нет, генерируем тихие ходы
    SimpleVector<Move> quiet_moves;
    generate_quiet_moves(board, player, quiet_moves);
    return quiet_moves;
}

SimpleVector<Move> MoveGenerator::generate_legal_moves(const Board& board) {
    return generate_legal_moves(board, board.turn());
}

bool MoveGenerator::has_legal_moves(const Board& board, Player player) {
    SimpleVector<Move> moves = generate_legal_moves(board, player);
    return !moves.empty();
}

SimpleVector<Move> MoveGenerator::get_moves_for_piece(const Board& board, Pos from) {
    SimpleVector<Move> all_moves = generate_legal_moves(board);
    SimpleVector<Move> piece_moves;
    for (size_t i = 0; i < all_moves.size(); ++i) {
        if (all_moves[i].from == from) {
            piece_moves.push_back(all_moves[i]);
        }
    }
    return piece_moves;
}

bool MoveGenerator::is_legal_move(const Board& board, const Move& move) {
    SimpleVector<Move> legal_moves = generate_legal_moves(board);
    for (size_t i = 0; i < legal_moves.size(); ++i) {
        if (legal_moves[i].from == move.from && legal_moves[i].to == move.to) {
            return true;
        }
    }
    return false;
}