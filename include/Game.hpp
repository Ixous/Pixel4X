#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Map.hpp"

class Game {

public:
    Game();
    ~Game();

    void gameloop();

    sf::RenderWindow window;
    sf::View mapview;

    Map map;
};