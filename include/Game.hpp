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

    void init();
    void gameloop();
    void handleEvent();
    void handleKeypress(sf::Keyboard::Key, bool);
    void tick();
    void draw();

    bool keypressed_Alt;

    sf::RenderWindow window;
    sf::View mapview;

    Map map;

    std::vector<Country> countries;
    std::vector<Nation> nations;
    std::vector<Province> provinces;
    std::vector<City> cities;
    void spawnCountries(unsigned short nbrNations);

    std::vector<Character> characters;

    std::vector<Culture> cultures;
    std::vector<Religion> religions;
};