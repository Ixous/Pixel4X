#pragma once

#include <SFML/Graphics.hpp>

class Cell {
public:
    Cell();
    ~Cell();
    unsigned short x,y;
    char height;
    unsigned short distanceToCoast;
    unsigned short continentSize;
    bool river;

    char temperature;
    unsigned char humidity;
    char windx, windy;
    unsigned char coverage;
    unsigned char rain;

    unsigned char fertility;
};