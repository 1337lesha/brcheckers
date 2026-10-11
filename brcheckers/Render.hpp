#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "SimpleVector.hpp"
#include "Board.hpp"
#include "Types.hpp"

enum class AppState {
    Menu,
    SideSelect,
    Playing
};

enum class GameMode {
    PvP,
    PvE
};

class Render {
public:
    static constexpr unsigned int TILE_SIZE = 80;
    static constexpr unsigned int BOARD_SIZE = TILE_SIZE * 8;
    static constexpr unsigned int STATUS_BAR_HEIGHT = 60;
    static constexpr unsigned int WINDOW_WIDTH = BOARD_SIZE;
    static constexpr unsigned int WINDOW_HEIGHT = BOARD_SIZE + STATUS_BAR_HEIGHT;

    explicit Render(sf::RenderWindow& window);

    // Отрисовка игрового поля
    void render_game(
        const Board& board,
        const std::optional<Pos>& selected_pos,
        const SimpleVector<Move>& valid_moves,
        GameStatus status,
        GameMode mode,
        Player human_side,
        bool is_ai_thinking
    );

    // Отрисовка главного меню (PvP / PvE)
    void render_menu(const sf::Vector2i& mouse_pos);

    // Отрисовка меню выбора стороны (Белые / Черные)
    void render_side_select(const sf::Vector2i& mouse_pos);

    // Хитбоксы кнопок главного меню
    static sf::FloatRect get_pvp_button_bounds();
    static sf::FloatRect get_pve_button_bounds();

    // Хитбоксы кнопок выбора цвета
    static sf::FloatRect get_white_button_bounds();
    static sf::FloatRect get_black_button_bounds();
    static sf::FloatRect get_back_button_bounds();

private:
    sf::RenderWindow& window_;
    sf::Font font_;
    bool font_loaded_ = false;

    // Цветовая палитра
    const sf::Color color_light_cell_{ 238, 214, 176 };
    const sf::Color color_dark_cell_{ 184, 135, 98 };
    const sf::Color color_selection_{ 245, 230, 80, 180 };
    const sf::Color color_target_hint_{ 100, 220, 100, 170 };
    const sf::Color color_capture_hint_{ 230, 60, 60, 190 };
    const sf::Color color_white_piece_{ 245, 245, 240 };
    const sf::Color color_white_piece_border_{ 180, 180, 170 };
    const sf::Color color_black_piece_{ 40, 40, 40 };
    const sf::Color color_black_piece_border_{ 15, 15, 15 };
    const sf::Color color_crown_{ 240, 190, 40 };
    const sf::Color color_status_bg_{ 30, 30, 35 };

    void draw_board();
    void draw_highlights(const std::optional<Pos>& selected_pos, const SimpleVector<Move>& valid_moves);
    void draw_pieces(const Board& board);
    void draw_piece(Pos p, Cell piece);
    void draw_status_bar(Player turn, GameStatus status, GameMode mode, Player human_side, bool is_ai_thinking);

    // Вспомогательная функция центрирования текста
    void draw_centered_text(const sf::String& str, unsigned int size, float center_x, float center_y, sf::Color color = sf::Color::White);
};