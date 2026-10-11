#include "GameController.hpp"

GameController::GameController()
    : window_(
        sf::VideoMode({ Render::WINDOW_WIDTH, Render::WINDOW_HEIGHT }),
        "Brazilian Checkers",
        sf::Style::Titlebar | sf::Style::Close
    ),
    renderer_(window_) {
    window_.setFramerateLimit(60);
}

void GameController::run() {
    while (window_.isOpen()) {
        process_events();

        if (app_state_ == AppState::Playing) {
            update_ai();
        }

        window_.clear();
        if (app_state_ == AppState::Menu) {
            renderer_.render_menu(sf::Mouse::getPosition(window_));
        }
        else if (app_state_ == AppState::SideSelect) {
            renderer_.render_side_select(sf::Mouse::getPosition(window_));
        }
        else {
            renderer_.render_game(
                board_,
                selected_pos_,
                valid_moves_for_selected_,
                game_status_,
                game_mode_,
                human_side_,
                is_ai_turn_pending_
            );
        }
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
                if (app_state_ == AppState::Menu) {
                    handle_menu_click(mouse->position.x, mouse->position.y);
                }
                else if (app_state_ == AppState::SideSelect) {
                    handle_side_select_click(mouse->position.x, mouse->position.y);
                }
                else {
                    handle_game_click(mouse->position.x, mouse->position.y);
                }
            }
        }
        else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape) {
                if (app_state_ == AppState::SideSelect) {
                    app_state_ = AppState::Menu;
                }
                else if (app_state_ == AppState::Playing) {
                    app_state_ = AppState::Menu;
                }
            }
            else if (key->code == sf::Keyboard::Key::R && app_state_ == AppState::Playing) {
                start_game();
            }
        }
    }
}

void GameController::handle_menu_click(int mouse_x, int mouse_y) {
    sf::Vector2f click_pos{ static_cast<float>(mouse_x), static_cast<float>(mouse_y) };

    if (Render::get_pvp_button_bounds().contains(click_pos)) {
        game_mode_ = GameMode::PvP;
        start_game();
        app_state_ = AppState::Playing;
    }
    else if (Render::get_pve_button_bounds().contains(click_pos)) {
        game_mode_ = GameMode::PvE;
        app_state_ = AppState::SideSelect;
    }
}

void GameController::handle_side_select_click(int mouse_x, int mouse_y) {
    sf::Vector2f click_pos{ static_cast<float>(mouse_x), static_cast<float>(mouse_y) };

    if (Render::get_white_button_bounds().contains(click_pos)) {
        human_side_ = Player::White;
        start_game();
        app_state_ = AppState::Playing;
    }
    else if (Render::get_black_button_bounds().contains(click_pos)) {
        human_side_ = Player::Black;
        start_game();
        app_state_ = AppState::Playing;
    }
    else if (Render::get_back_button_bounds().contains(click_pos)) {
        app_state_ = AppState::Menu;
    }
}

void GameController::start_game() {
    board_.reset();
    selected_pos_.reset();
    valid_moves_for_selected_.clear();
    game_status_ = GameStatus::InProgress;
    is_ai_turn_pending_ = false;

    // Если игра против бота и игрок выбрал черных — первым ходит бот (белые)
    if (game_mode_ == GameMode::PvE && human_side_ == Player::Black) {
        is_ai_turn_pending_ = true;
        ai_clock_.restart();
    }
}

void GameController::handle_game_click(int mouse_x, int mouse_y) {
    if (game_status_ != GameStatus::InProgress) return;

    // В PvE блокируем клики, когда очередь бота
    if (game_mode_ == GameMode::PvE) {
        Player ai_side = (human_side_ == Player::White) ? Player::Black : Player::White;
        if (board_.turn() == ai_side) return;
    }

    if (mouse_y >= static_cast<int>(Render::BOARD_SIZE) || mouse_x >= static_cast<int>(Render::BOARD_SIZE)) {
        return;
    }

    int8_t col = static_cast<int8_t>(mouse_x / Render::TILE_SIZE);
    int8_t row = static_cast<int8_t>(mouse_y / Render::TILE_SIZE);
    Pos clicked_pos{ row, col };

    // 1. Попытка совершить ход в выбранную клетку
    if (selected_pos_.has_value()) {
        for (size_t i = 0; i < valid_moves_for_selected_.size(); ++i) {
            const Move& m = valid_moves_for_selected_[i];
            if (m.to == clicked_pos) {
                board_.apply_move(m);
                selected_pos_.reset();
                valid_moves_for_selected_.clear();
                check_game_over();

                // Если партия продолжается и сейчас очередь бота
                if (game_mode_ == GameMode::PvE && game_status_ == GameStatus::InProgress) {
                    Player ai_side = (human_side_ == Player::White) ? Player::Black : Player::White;
                    if (board_.turn() == ai_side) {
                        is_ai_turn_pending_ = true;
                        ai_clock_.restart();
                    }
                }
                return;
            }
        }
    }

    // 2. Выбор фигуры
    Cell clicked_cell = board_.get(clicked_pos);
    if (Board::is_friendly(clicked_cell, board_.turn())) {
        SimpleVector<Move> moves = MoveGenerator::get_moves_for_piece(board_, clicked_pos);
        if (!moves.empty()) {
            selected_pos_ = clicked_pos;
            valid_moves_for_selected_ = moves;
            return;
        }
    }

    selected_pos_.reset();
    valid_moves_for_selected_.clear();
}

void GameController::update_ai() {
    if (!is_ai_turn_pending_ || game_status_ != GameStatus::InProgress) return;

    // Небольшая задержка (300 мс), чтобы ход не происходил со скоростью 1 кадра
    if (ai_clock_.getElapsedTime().asMilliseconds() < 300) return;

    constexpr int SEARCH_DEPTH = 6;
    std::optional<Move> best = Minimax::find_best_move(board_, SEARCH_DEPTH);

    if (best.has_value()) {
        board_.apply_move(*best);
    }

    is_ai_turn_pending_ = false;
    check_game_over();
}

void GameController::check_game_over() {
    if (!MoveGenerator::has_legal_moves(board_, board_.turn())) {
        game_status_ = (board_.turn() == Player::White) ? GameStatus::BlackWon : GameStatus::WhiteWon;
    }
}