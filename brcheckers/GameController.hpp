#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Minimax.hpp"
#include "SimpleVector.hpp"
#include "Board.hpp"
#include "MoveGenerator.hpp"
#include "Types.hpp"
#include "Render.hpp"

class GameController {
public:
    GameController();
    void run();

private:
    sf::RenderWindow window_;
    Render renderer_;
    Board board_;

    AppState app_state_ = AppState::Menu;
    GameMode game_mode_ = GameMode::PvP;
    Player human_side_ = Player::White;

    std::optional<Pos> selected_pos_;
    SimpleVector<Move> valid_moves_for_selected_;
    GameStatus game_status_ = GameStatus::InProgress;

    sf::Clock ai_clock_;
    bool is_ai_turn_pending_ = false;

    void process_events();
    void handle_menu_click(int mouse_x, int mouse_y);
    void handle_side_select_click(int mouse_x, int mouse_y);
    void handle_game_click(int mouse_x, int mouse_y);
    void update_ai();
    void check_game_over();
    void start_game();
};