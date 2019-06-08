#pragma once

#include <vector>
#include <cmath>

#include "Cell.hpp"

enum class Mapmode {
    HEIGHT, TEMPERATURE, HUMIDITY, FERTILITY,
    DEBUG_CONTINENTSIZE, DEBUG_DISTANCETOCOAST
};

class Map {
public:
    Map(unsigned short _width, unsigned short _height, int _seed);
    ~Map();
    Map* map;

    unsigned short getHeight() {return height;}
    unsigned short getWidth() {return width;}
    Cell getCell(unsigned short xxx, unsigned short yyy) {return cells[xxx][yyy];}

    void generate();
    void update();
    void mapmode(Mapmode _mapmode);
    void draw(sf::RenderWindow*);

private:
    unsigned short height, width;
    int seed;

    std::vector<std::vector<Cell>> cells;
    std::vector<Cell*> coast;

    void genHeightMap();
    void markCoast();
    void calcDistanceToCoast();
    void calcDistanceToCoast_iter(std::vector<Cell*> layer, unsigned short distance);
    void calcContinentSize();

    void genRivers();
    void spreadRiver(unsigned short x, unsigned short y, short inertia_x, short inertia_y);

    void genTemperature();
    void genHumidity();

    void genFertility();

    sf::Image mapimage;
    sf::Texture maptexture;
    sf::Sprite mapsprite;
    void mapmodeHeight();
    void mapmodeTemperature();
    void mapmodeHumidity();
    void mapmodeFertility();
    void mapmodeDistanceToCoast();
    void mapmodeContinentSize();

    void updateHumidity();
};