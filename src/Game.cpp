#include "Game.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Game::Game() :
window (sf::VideoMode(2048, 1024+512), "Pixel4X") , mapview(sf::FloatRect(0,0,1024,512+256)), map(1024,512+256, 1)
{
}

Game::~Game() {
}

void Game::gameloop() {
    map.genHeightMap();
    map.markCoast();
    map.selectHeightMap();
    map.genRivers();

    while (window.isOpen()) {
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
        window.clear(sf::Color::Black);
        window.setView(mapview);
        map.draw(&window);
        window.display();
    }
}
