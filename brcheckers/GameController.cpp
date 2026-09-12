#include "GameController.hpp"

GameController::GameController()
    : window_(
        sf::VideoMode({ Renderer::WINDOW_WIDTH, Renderer::WINDOW_HEIGHT }),
        "Brazilian Checkers - Hotseat",
        sf::Style::Titlebar | sf::Style::Close
    ),
    renderer_(window_) {
    window_.setFramerateLimit(60);
}

void GameController::run() {
    while (window_.isOpen()) {
        process_events();

        window_.clear();
        renderer_.render(board_, selected_pos_, valid_moves_for_selected_, game_status_);
        window_.display();
    }
}

void GameController::process_events() {
    
    while (const std::optional<sf::Event> event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        }
        else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse->button == sf::Mouse::Button::Left) {
                handle_mouse_click(mouse->position.x, mouse->position.y);
            }
        }
        else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::R) {
                reset_game();
            }
        }
    }
}

void GameController::handle_mouse_click(int mouse_x, int mouse_y) {
    if (game_status_ != GameStatus::InProgress) return;

    // Клик за пределами шахматного поля (например, в статус-баре)
    if (mouse_y >= static_cast<int>(Renderer::BOARD_SIZE) || mouse_x >= static_cast<int>(Renderer::BOARD_SIZE)) {
        return;
    }

    int8_t col = static_cast<int8_t>(mouse_x / Renderer::TILE_SIZE);
    int8_t row = static_cast<int8_t>(mouse_y / Renderer::TILE_SIZE);
    Pos clicked_pos{ row, col };

    //  Если шашка уже выбрана, проверяем клик по целевым клеткам доступных ходов
    if (selected_pos_.has_value()) {
        for (size_t i = 0; i < valid_moves_for_selected_.size(); ++i) {
            const Move& m = valid_moves_for_selected_[i];
            if (m.to == clicked_pos) {
                board_.apply_move(m);

                selected_pos_.reset();
                valid_moves_for_selected_.clear();

                check_game_over();
                return;
            }
        }
    }

    //  Клик по своей фигуре для выбора / смены выбора
    Cell clicked_cell = board_.get(clicked_pos);
    if (Board::is_friendly(clicked_cell, board_.turn())) {
        // MoveGenerator::get_moves_for_piece уже фильтрует ходы по правилу большинства
        SimpleVector<Move> moves = MoveGenerator::get_moves_for_piece(board_, clicked_pos);
        if (!moves.empty()) {
            selected_pos_ = clicked_pos;
            valid_moves_for_selected_ = moves;
            return;
        }
    }

    //  Клик в пустоту или по фигуре без доступных ходов сбрасывает выбор
    selected_pos_.reset();
    valid_moves_for_selected_.clear();
}

void GameController::check_game_over() {
    if (!MoveGenerator::has_legal_moves(board_, board_.turn())) {
        if (board_.turn() == Player::White) {
            game_status_ = GameStatus::BlackWon;
        }
        else {
            game_status_ = GameStatus::WhiteWon;
        }
    }
}

void GameController::reset_game() {
    board_.reset();
    selected_pos_.reset();
    valid_moves_for_selected_.clear();
    game_status_ = GameStatus::InProgress;
}