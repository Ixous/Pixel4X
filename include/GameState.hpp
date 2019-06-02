#pragma once

#include <SFML/System.hpp>

class GameState {
public:
    virtual void init() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void handleEvent(sf::Event, sf::RenderWindow) = 0;
    virtual void draw(sf::RenderWindow) = 0;
};
