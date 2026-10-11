#include "Render.hpp"

Render::Render(sf::RenderWindow& window) : window_(window) {
    // 1. Пробуем загрузить шрифт из папки программы
    if (font_.openFromFile("arial.ttf")) {
        font_loaded_ = true;
    }
    // 2. Если не найден, берем системный Arial из Windows
    else if (font_.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        font_loaded_ = true;
    }
}

// Границы кнопок главного меню
sf::FloatRect Render::get_pvp_button_bounds() {
    return sf::FloatRect({ 120.0f, 220.0f }, { 400.0f, 110.0f });
}

sf::FloatRect Render::get_pve_button_bounds() {
    return sf::FloatRect({ 120.0f, 370.0f }, { 400.0f, 110.0f });
}

// Границы кнопок меню выбора стороны
sf::FloatRect Render::get_white_button_bounds() {
    return sf::FloatRect({ 120.0f, 200.0f }, { 400.0f, 95.0f });
}

sf::FloatRect Render::get_black_button_bounds() {
    return sf::FloatRect({ 120.0f, 320.0f }, { 400.0f, 95.0f });
}

sf::FloatRect Render::get_back_button_bounds() {
    return sf::FloatRect({ 220.0f, 450.0f }, { 200.0f, 60.0f });
}

void Render::draw_centered_text(const sf::String& str, unsigned int size, float center_x, float center_y, sf::Color color) {
    if (!font_loaded_) return;

    sf::Text text(font_);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);

    // Центрирование через границы в SFML 3
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f });
    text.setPosition({ center_x, center_y });

    window_.draw(text);
}

void Render::render_menu(const sf::Vector2i& mouse_pos) {
    sf::RectangleShape bg({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) });
    bg.setFillColor(sf::Color(26, 28, 34));
    window_.draw(bg);

    // Заголовок игры
    draw_centered_text("BRAZILIAN CHECKERS", 32, WINDOW_WIDTH / 2.0f, 100.0f, color_crown_);
    draw_centered_text("Select Game Mode", 18, WINDOW_WIDTH / 2.0f, 145.0f, sf::Color(180, 180, 190));

    sf::Vector2f mpos{ static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y) };
    auto pvp_bounds = get_pvp_button_bounds();
    auto pve_bounds = get_pve_button_bounds();

    bool hover_pvp = pvp_bounds.contains(mpos);
    bool hover_pve = pve_bounds.contains(mpos);

    // Кнопка PvP
    sf::RectangleShape btn_pvp(pvp_bounds.size);
    btn_pvp.setPosition(pvp_bounds.position);
    btn_pvp.setFillColor(hover_pvp ? sf::Color(55, 62, 78) : sf::Color(38, 42, 54));
    btn_pvp.setOutlineThickness(hover_pvp ? 3.0f : 1.5f);
    btn_pvp.setOutlineColor(hover_pvp ? sf::Color(100, 200, 255) : sf::Color(80, 85, 100));
    window_.draw(btn_pvp);

    float pvp_cx = pvp_bounds.position.x + pvp_bounds.size.x / 2.0f;
    float pvp_cy = pvp_bounds.position.y + pvp_bounds.size.y / 2.0f;
    draw_centered_text("PvP", 34, pvp_cx, pvp_cy - 12.0f, sf::Color::White);
    draw_centered_text("Player vs Player", 15, pvp_cx, pvp_cy + 24.0f, sf::Color(180, 190, 200));

    // Кнопка PvE
    sf::RectangleShape btn_pve(pve_bounds.size);
    btn_pve.setPosition(pve_bounds.position);
    btn_pve.setFillColor(hover_pve ? sf::Color(55, 62, 78) : sf::Color(38, 42, 54));
    btn_pve.setOutlineThickness(hover_pve ? 3.0f : 1.5f);
    btn_pve.setOutlineColor(hover_pve ? sf::Color(100, 200, 255) : sf::Color(80, 85, 100));
    window_.draw(btn_pve);

    float pve_cx = pve_bounds.position.x + pve_bounds.size.x / 2.0f;
    float pve_cy = pve_bounds.position.y + pve_bounds.size.y / 2.0f;
    draw_centered_text("PvE", 34, pve_cx, pve_cy - 12.0f, sf::Color::White);
    draw_centered_text("Player vs AI (Minimax)", 15, pve_cx, pve_cy + 24.0f, sf::Color(180, 190, 200));
}

void Render::render_side_select(const sf::Vector2i& mouse_pos) {
    sf::RectangleShape bg({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) });
    bg.setFillColor(sf::Color(26, 28, 34));
    window_.draw(bg);

    draw_centered_text("CHOOSE YOUR SIDE", 30, WINDOW_WIDTH / 2.0f, 90.0f, color_crown_);
    draw_centered_text("Who makes the first move?", 18, WINDOW_WIDTH / 2.0f, 135.0f, sf::Color(180, 180, 190));

    sf::Vector2f mpos{ static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y) };
    auto white_bounds = get_white_button_bounds();
    auto black_bounds = get_black_button_bounds();
    auto back_bounds = get_back_button_bounds();

    bool hover_white = white_bounds.contains(mpos);
    bool hover_black = black_bounds.contains(mpos);
    bool hover_back = back_bounds.contains(mpos);

    // Кнопка «Белые»
    sf::RectangleShape btn_w(white_bounds.size);
    btn_w.setPosition(white_bounds.position);
    btn_w.setFillColor(hover_white ? sf::Color(65, 70, 85) : sf::Color(42, 46, 58));
    btn_w.setOutlineThickness(hover_white ? 3.0f : 1.5f);
    btn_w.setOutlineColor(hover_white ? sf::Color(245, 245, 240) : sf::Color(100, 105, 120));
    window_.draw(btn_w);

    float w_cx = white_bounds.position.x + white_bounds.size.x / 2.0f;
    float w_cy = white_bounds.position.y + white_bounds.size.y / 2.0f;
    draw_centered_text("WHITE", 24, w_cx, w_cy - 12.0f, color_white_piece_);
    draw_centered_text("(You move first)", 15, w_cx, w_cy + 18.0f, sf::Color(180, 220, 180));

    // Кнопка «Черные»
    sf::RectangleShape btn_b(black_bounds.size);
    btn_b.setPosition(black_bounds.position);
    btn_b.setFillColor(hover_black ? sf::Color(65, 70, 85) : sf::Color(42, 46, 58));
    btn_b.setOutlineThickness(hover_black ? 3.0f : 1.5f);
    btn_b.setOutlineColor(hover_black ? sf::Color(200, 160, 50) : sf::Color(100, 105, 120));
    window_.draw(btn_b);

    float b_cx = black_bounds.position.x + black_bounds.size.x / 2.0f;
    float b_cy = black_bounds.position.y + black_bounds.size.y / 2.0f;
    draw_centered_text("BLACK", 24, b_cx, b_cy - 12.0f, sf::Color(200, 200, 200));
    draw_centered_text("(AI moves first)", 15, b_cx, b_cy + 18.0f, sf::Color(220, 160, 160));

    // Кнопка «Назад»
    sf::RectangleShape btn_back(back_bounds.size);
    btn_back.setPosition(back_bounds.position);
    btn_back.setFillColor(hover_back ? sf::Color(80, 45, 45) : sf::Color(55, 35, 35));
    btn_back.setOutlineThickness(hover_back ? 2.5f : 1.0f);
    btn_back.setOutlineColor(hover_back ? sf::Color(230, 100, 100) : sf::Color(120, 70, 70));
    window_.draw(btn_back);

    float back_cx = back_bounds.position.x + back_bounds.size.x / 2.0f;
    float back_cy = back_bounds.position.y + back_bounds.size.y / 2.0f;
    draw_centered_text("BACK", 20, back_cx, back_cy, sf::Color(230, 200, 200));
}

void Render::render_game(
    const Board& board,
    const std::optional<Pos>& selected_pos,
    const SimpleVector<Move>& valid_moves,
    GameStatus status,
    GameMode mode,
    Player human_side,
    bool is_ai_thinking
) {
    draw_board();
    draw_highlights(selected_pos, valid_moves);
    draw_pieces(board);
    draw_status_bar(board.turn(), status, mode, human_side, is_ai_thinking);
}

void Render::draw_board() {
    sf::RectangleShape cell_shape({ static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            cell_shape.setPosition({ static_cast<float>(c * TILE_SIZE), static_cast<float>(r * TILE_SIZE) });
            cell_shape.setFillColor((r + c) % 2 == 0 ? color_light_cell_ : color_dark_cell_);
            window_.draw(cell_shape);
        }
    }
}

void Render::draw_highlights(const std::optional<Pos>& selected_pos, const SimpleVector<Move>& valid_moves) {
    if (selected_pos.has_value()) {
        sf::RectangleShape select_box({ static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) });
        select_box.setPosition({
            static_cast<float>(selected_pos->c * TILE_SIZE),
            static_cast<float>(selected_pos->r * TILE_SIZE)
            });
        select_box.setFillColor(color_selection_);
        window_.draw(select_box);
    }

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

void Render::draw_pieces(const Board& board) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Pos p{ static_cast<int8_t>(r), static_cast<int8_t>(c) };
            Cell cell = board.get(p);
            if (cell != Cell::Empty) draw_piece(p, cell);
        }
    }
}

void Render::draw_piece(Pos p, Cell piece) {
    float center_x = p.c * TILE_SIZE + TILE_SIZE / 2.0f;
    float center_y = p.r * TILE_SIZE + TILE_SIZE / 2.0f;
    float radius = (TILE_SIZE / 2.0f) - 8.0f;

    sf::CircleShape body(radius);
    body.setOrigin({ radius, radius });
    body.setPosition({ center_x, center_y });
    body.setOutlineThickness(3.0f);

    bool white = Board::is_white(piece);
    body.setFillColor(white ? color_white_piece_ : color_black_piece_);
    body.setOutlineColor(white ? color_white_piece_border_ : color_black_piece_border_);
    window_.draw(body);

    float inner_radius = radius * 0.72f;
    sf::CircleShape inner_ring(inner_radius);
    inner_ring.setOrigin({ inner_radius, inner_radius });
    inner_ring.setPosition({ center_x, center_y });
    inner_ring.setFillColor(sf::Color::Transparent);
    inner_ring.setOutlineThickness(1.5f);
    inner_ring.setOutlineColor(white ? color_white_piece_border_ : color_black_piece_border_);
    window_.draw(inner_ring);

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

void Render::draw_status_bar(Player turn, GameStatus status, GameMode mode, Player human_side, bool is_ai_thinking) {
    sf::RectangleShape bar({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(STATUS_BAR_HEIGHT) });
    bar.setPosition({ 0.0f, static_cast<float>(BOARD_SIZE) });
    bar.setFillColor(color_status_bg_);
    window_.draw(bar);

    float bar_cy = BOARD_SIZE + STATUS_BAR_HEIGHT / 2.0f;

    // Индикатор текущего цвета хода (шашка слева)
    float ind_radius = 14.0f;
    sf::CircleShape turn_indicator(ind_radius);
    turn_indicator.setOrigin({ ind_radius, ind_radius });
    turn_indicator.setPosition({ 35.0f, bar_cy });
    turn_indicator.setOutlineThickness(2.0f);

    bool white_turn = (turn == Player::White);
    turn_indicator.setFillColor(white_turn ? color_white_piece_ : color_black_piece_);
    turn_indicator.setOutlineColor(white_turn ? color_white_piece_border_ : sf::Color::White);
    window_.draw(turn_indicator);

    // Текстовый статус партии
    if (status == GameStatus::InProgress) {
        if (mode == GameMode::PvE && is_ai_thinking) {
            draw_centered_text("AI is thinking...", 18, WINDOW_WIDTH / 2.0f, bar_cy, sf::Color(240, 180, 50));
        }
        else {
            const char* turn_str = (turn == Player::White) ? "Turn: White" : "Turn: Black";
            draw_centered_text(turn_str, 18, WINDOW_WIDTH / 2.0f, bar_cy, sf::Color::White);
        }
    }
    else {
        if (status == GameStatus::WhiteWon) {
            draw_centered_text("WHITE WON!", 22, WINDOW_WIDTH / 2.0f, bar_cy, color_crown_);
        }
        else if (status == GameStatus::BlackWon) {
            draw_centered_text("BLACK WON!", 22, WINDOW_WIDTH / 2.0f, bar_cy, color_crown_);
        }
        else {
            draw_centered_text("DRAW", 22, WINDOW_WIDTH / 2.0f, bar_cy, sf::Color::White);
        }
    }

    // Подсказка выхода в меню
    draw_centered_text("[ESC] Menu", 13, WINDOW_WIDTH - 55.0f, bar_cy, sf::Color(150, 150, 160));
}