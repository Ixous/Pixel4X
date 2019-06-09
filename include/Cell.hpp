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

    char temperature; // multiply by 2 to get temperature in °C
    unsigned char humidity;
    char windx, windy;
    unsigned char cloud;
    unsigned char rain;

    unsigned char fertility;
};