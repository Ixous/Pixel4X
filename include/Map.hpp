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
    std::vector<std::vector<unsigned int>> distanceToCoast;

    int seed;
    void genHeightMap();
    void markCoast();
    void calcDistanceToCoast();
    void calcDistanceToCoast_iter(std::vector<Cell*> layer, unsigned short distance);
    void calcContinentSize();

    void genRivers();
    void spreadRiver(unsigned short x, unsigned short y);
    void spreadRiver2(unsigned short x, unsigned short y, short inertia_x, short inertia_y);

    sf::Image mapimage;
    sf::Texture maptexture;
    sf::Sprite mapsprite;
    void draw(sf::RenderWindow*);
    void selectHeightMap();
    void selectDistanceToCoastMap();
    void selectContinentSizeMap();
};