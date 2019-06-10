#include "Game.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Game::Game() :
window (sf::VideoMode(2048, 1024+512), "Pixel4X") , mapview(sf::FloatRect(0,0,1024,512+256)), map(1024,512+256, 1)
{
    keypressed_Alt = false;
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
            case sf::Event::KeyPressed:
                handleKeypress(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeypress(event.key.code, false);
                break;
            default:
                break;
        }
    }
}

void Game::handleKeypress(sf::Keyboard::Key key, bool pressed) {
    if (window.hasFocus()) {
        switch (key) {
        case sf::Keyboard::Key::Enter:
            tick();
            break;
        case sf::Keyboard::Key::P:
            map.debug_probetemp();
            break;
        case sf::Keyboard::Key::LAlt:
            keypressed_Alt = pressed;
            break;
        case sf::Keyboard::Key::Num1:
            if (keypressed_Alt) map.mapmode(Mapmode::HEIGHT);
            break;
        case sf::Keyboard::Key::Num2:
            if (keypressed_Alt) map.mapmode(Mapmode::TEMPERATURE);
            break;
        case sf::Keyboard::Key::Num3:
            if (keypressed_Alt) map.mapmode(Mapmode::HUMIDITY);
            break;
        case sf::Keyboard::Key::Num4:
            if (keypressed_Alt) map.mapmode(Mapmode::WIND);
            break;
        case sf::Keyboard::Key::Num5:
            if (keypressed_Alt) map.mapmode(Mapmode::CLOUD);
            break;
        case sf::Keyboard::Key::Num6:
            if (keypressed_Alt) map.mapmode(Mapmode::RAIN);
            break;
        case sf::Keyboard::Key::Num7:
            if (keypressed_Alt) map.mapmode(Mapmode::FERTILITY);
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