#include "Game.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Game::Game() :
game(this) , window (sf::VideoMode(2048, 1024+512), "Pixel4X") , mapview(sf::FloatRect(0,0,1024,512+256)), map(1024,512+256, 1)
{
}

Game::~Game() {
}

void Game::gameloop() {
    init();
    while (window.isOpen()) {
        handleEvent();
        draw();
    }
}

void Game::init() {
    map.generate();
    map.mapmode(Mapmode::HEIGHT);
}

void Game::handleEvent() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
        }
    }
}

void Game::tick() {
    map.update();
}

void Game::draw() {
    window.clear(sf::Color::Black);
    window.setView(mapview);
    map.draw(&window);
    window.display();
}