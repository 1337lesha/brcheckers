#include <SFML/Graphics.hpp>
#include <optional>
#include "SimpleVector.hpp"
#include "Board.hpp"
#include "Types.hpp"

class Renderer {
public:
    static constexpr unsigned int TILE_SIZE = 80;
    static constexpr unsigned int BOARD_SIZE = TILE_SIZE * 8;
    static constexpr unsigned int STATUS_BAR_HEIGHT = 60;
    static constexpr unsigned int WINDOW_WIDTH = BOARD_SIZE;
    static constexpr unsigned int WINDOW_HEIGHT = BOARD_SIZE + STATUS_BAR_HEIGHT;

    explicit Renderer(sf::RenderWindow& window);

    void render(
        const Board& board,
        const std::optional<Pos>& selected_pos,
        const SimpleVector<Move>& valid_moves,
        GameStatus status
    );

private:
    sf::RenderWindow& window_;

    // Палитра цветов
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
    void draw_status_bar(Player turn, GameStatus status);
};