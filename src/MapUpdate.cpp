#include "Map.hpp"

#include <cmath> //sin() , atan() , pow()
#include <stdlib.h> //rand()
#include <iostream>

#define PI 3.14159265358979323846


void Map::updateHumidity() {
    std::vector<std::vector<unsigned char>> newHumidity(width-2, std::vector<unsigned char>(height-2));
    for (unsigned short xxx = 1; xxx < width-1; xxx++) {
        for (unsigned short yyy = 1; yyy < height-1; yyy++) {
            if (cells[xxx][yyy].height>=0) {
                unsigned short neightbourHumidity = 0;
                for (auto cell : getNeighbours(&cells[xxx][yyy], 1, true)) neightbourHumidity += cell->humidity;
                neightbourHumidity /= 8;
                newHumidity[xxx-1][yyy-1] = 0.1*cells[xxx][yyy].humidity + 0.9*neightbourHumidity;
            } else newHumidity[xxx-1][yyy-1] = cells[xxx][yyy].humidity;
        }
    }
    for (unsigned short xxx = 1; xxx < width-1; xxx++) {
        for (unsigned short yyy = 1; yyy < height-1; yyy++) {
            cells[xxx][yyy].humidity = newHumidity[xxx-1][yyy-1];
        }
    }
}