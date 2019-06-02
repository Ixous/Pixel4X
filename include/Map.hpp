#pragma once

#include <vector>
#include <cmath>

#include "Cell.hpp"

class Map {

public:
    Map(unsigned short _width, unsigned short _height, int _seed);
    ~Map();

    unsigned short height, width;

    std::vector<std::vector<Cell>> cells;
    std::vector<Cell*> coast;

    int seed;
    void genHeightMap();
    void markCoast();
    void genRivers();
    void spreadRiver(unsigned short x, unsigned short y);

    sf::Image mapimage;
    sf::Texture maptexture;
    sf::Sprite mapsprite;
    void draw(sf::RenderWindow*);
    void selectHeightMap();
};