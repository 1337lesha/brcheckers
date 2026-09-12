cmake_minimum_required(VERSION 3.20)
project(BrazilianCheckers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(checkers_game
    src/core/Board.cpp
    src/core/MoveGenerator.cpp
    src/ui/Render.cpp
    src/ui/GameController.cpp
    src/main.cpp
)

target_link_libraries(checkers_game PRIVATE SFML::Graphics SFML::Window SFML::System)