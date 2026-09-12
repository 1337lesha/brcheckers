#include "Render.hpp"

Renderer::Renderer(sf::RenderWindow& window) : window_(window) {}

void Renderer::render(
    const Board& board,
    const std::optional<Pos>& selected_pos,
    const SimpleVector<Move>& valid_moves,
    GameStatus status
) {
    draw_board();
    draw_highlights(selected_pos, valid_moves);
    draw_pieces(board);
    draw_status_bar(board.turn(), status);
}

void Renderer::draw_board() {
    sf::RectangleShape cell_shape({ static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            cell_shape.setPosition({ static_cast<float>(c * TILE_SIZE), static_cast<float>(r * TILE_SIZE) });
            if ((r + c) % 2 == 0) {
                cell_shape.setFillColor(color_light_cell_);
            }
            else {
                cell_shape.setFillColor(color_dark_cell_);
            }
            window_.draw(cell_shape);
        }
    }
}

void Renderer::draw_highlights(const std::optional<Pos>& selected_pos, const SimpleVector<Move>& valid_moves) {
    // Подсветка выбранной шашки
    if (selected_pos.has_value()) {
        sf::RectangleShape select_box({ static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
        select_box.setPosition({
            static_cast<float>(selected_pos->c * TILE_SIZE),
            static_cast<float>(selected_pos->r * TILE_SIZE)
            });
        select_box.setFillColor(color_selection_);
        window_.draw(select_box);
    }

    // Подсветка целевых полей (зеленые маркеры для тихих ходов, красные для взятий)
    for (size_t i = 0; i < valid_moves.size(); ++i) {
        const Move& m = valid_moves[i];
        float center_x = m.to.c * TILE_SIZE + TILE_SIZE / 2.0f;
        float center_y = m.to.r * TILE_SIZE + TILE_SIZE / 2.0f;

        float radius = m.is_capture() ? 14.0f : 10.0f;
        sf::CircleShape hint(radius);
        hint.setOrigin({ radius, radius });
        hint.setPosition({ center_x, center_y });
        hint.setFillColor(m.is_capture() ? color_capture_hint_ : color_target_hint_);

        window_.draw(hint);
    }
}

void Renderer::draw_pieces(const Board& board) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Pos p{ static_cast<int8_t>(r), static_cast<int8_t>(c) };
            Cell cell = board.get(p);
            if (cell != Cell::Empty) {
                draw_piece(p, cell);
            }
        }
    }
}

void Renderer::draw_piece(Pos p, Cell piece) {
    float center_x = p.c * TILE_SIZE + TILE_SIZE / 2.0f;
    float center_y = p.r * TILE_SIZE + TILE_SIZE / 2.0f;
    float radius = (TILE_SIZE / 2.0f) - 8.0f;

    // Основное тело шашки
    sf::CircleShape body(radius);
    body.setOrigin({ radius, radius });
    body.setPosition({ center_x, center_y });
    body.setOutlineThickness(3.0f);

    bool white = Board::is_white(piece);
    body.setFillColor(white ? color_white_piece_ : color_black_piece_);
    body.setOutlineColor(white ? color_white_piece_border_ : color_black_piece_border_);
    window_.draw(body);

    // Внутренняя рельефная фаска шашки
    float inner_radius = radius * 0.72f;
    sf::CircleShape inner_ring(inner_radius);
    inner_ring.setOrigin({ inner_radius, inner_radius });
    inner_ring.setPosition({ center_x, center_y });
    inner_ring.setFillColor(sf::Color::Transparent);
    inner_ring.setOutlineThickness(1.5f);
    inner_ring.setOutlineColor(white ? color_white_piece_border_ : color_black_piece_border_);
    window_.draw(inner_ring);

    // Знак дамки (коронный золотой диск по центру)
    if (Board::is_king(piece)) {
        float king_radius = radius * 0.38f;
        sf::CircleShape crown(king_radius);
        crown.setOrigin({ king_radius, king_radius });
        crown.setPosition({ center_x, center_y });
        crown.setFillColor(color_crown_);
        crown.setOutlineThickness(2.0f);
        crown.setOutlineColor(sf::Color(160, 110, 10));
        window_.draw(crown);
    }
}

void Renderer::draw_status_bar(Player turn, GameStatus status) {
    // Фоновая плашка статус-бара
    sf::RectangleShape bar({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(STATUS_BAR_HEIGHT) });
    bar.setPosition({ 0.0f, static_cast<float>(BOARD_SIZE) });
    bar.setFillColor(color_status_bg_);
    window_.draw(bar);

    // Индикатор текущего хода
    float ind_radius = 14.0f;
    sf::CircleShape turn_indicator(ind_radius);
    turn_indicator.setOrigin({ ind_radius, ind_radius });
    turn_indicator.setPosition({ 35.0f, BOARD_SIZE + STATUS_BAR_HEIGHT / 2.0f });
    turn_indicator.setOutlineThickness(2.0f);

    if (status == GameStatus::InProgress) {
        bool white_turn = (turn == Player::White);
        turn_indicator.setFillColor(white_turn ? color_white_piece_ : color_black_piece_);
        turn_indicator.setOutlineColor(white_turn ? color_white_piece_border_ : color_white_piece_);
    }
    else {
        // При победе показываем цвет победителя
        if (status == GameStatus::WhiteWon) {
            turn_indicator.setFillColor(color_white_piece_);
            turn_indicator.setOutlineColor(color_crown_);
        }
        else if (status == GameStatus::BlackWon) {
            turn_indicator.setFillColor(color_black_piece_);
            turn_indicator.setOutlineColor(color_crown_);
        }
        else {
            turn_indicator.setFillColor(sf::Color(120, 120, 120));
            turn_indicator.setOutlineColor(sf::Color::White);
        }
    }
    window_.draw(turn_indicator);

    // Индикатор состояния игры (прямоугольная метка статуса)
    sf::RectangleShape status_badge({ 18.0f, 18.0f });
    status_badge.setOrigin({ 9.0f, 9.0f });
    status_badge.setPosition({ static_cast<float>(WINDOW_WIDTH - 35), BOARD_SIZE + STATUS_BAR_HEIGHT / 2.0f });

    if (status == GameStatus::InProgress) {
        status_badge.setFillColor(sf::Color(80, 200, 120)); // Зеленый: игра активна
    }
    else {
        status_badge.setFillColor(sf::Color(220, 60, 60));  // Красный: игра завершена
    }
    window_.draw(status_badge);
}