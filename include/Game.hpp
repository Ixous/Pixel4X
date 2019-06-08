#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Map.hpp"
#include "Country.hpp"
#include "Nation.hpp"
#include "Province.hpp"
#include "City.hpp"
#include "Character.hpp"
#include "Culture.hpp"
#include "Religion.hpp"

class Game {

public:
    Game();
    ~Game();
    Game* game;

    void init();
    void gameloop();
    void handleEvent();
    void tick();
    void draw();

    sf::RenderWindow window;
    sf::View mapview;

    Map map;

    std::vector<Country> countries;
    std::vector<Nation> nations;
    std::vector<Province> provinces;
    std::vector<City> cities;

    std::vector<Character> characters;

    std::vector<Culture> cultures;
    std::vector<Religion> religions;
};