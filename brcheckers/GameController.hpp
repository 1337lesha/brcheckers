#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
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
    Renderer renderer_;
    Board board_;

    std::optional<Pos> selected_pos_;
    SimpleVector<Move> valid_moves_for_selected_;
    GameStatus game_status_ = GameStatus::InProgress;

    void process_events();
    void handle_mouse_click(int mouse_x, int mouse_y);
    void check_game_over();
    void reset_game();
};