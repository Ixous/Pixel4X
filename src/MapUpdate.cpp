#include "Map.hpp"

#include "Utils.hpp"

#include <cmath> //sin() , atan() , pow()
#include <stdlib.h> //rand()
#include <iostream>

#define PI 3.14159265358979323846

#define UPDATE_TEMPERATURE_CONVECTION_MAXDISTANCE 10
#define UPDATE_TEMPERATURE_CONDUCTION_AREA 1
#define UPDATE_TEMPERATURE_SUNLIGHT_MULTIPLIER 2
#define UPDATE_TEMPERATURE_SUNLIGHT_COVERAGE_POWER 2
#define UPDATE_TEMPERATURE_SUNLIGHT_LATITUDE_POWER 1
#define UPDATE_TEMPERATURE_RAIN_MULTIPLIER 0.4

void Map::updateTemperature() {
    std::vector<std::vector<unsigned char>> oldTemperature(width, std::vector<unsigned char>(height));
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            oldTemperature[xxx][yyy] = cells[xxx][yyy].temperature;
        }
    }
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            // Convection
            int xxx2 = xxx + UPDATE_TEMPERATURE_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windx/128.0;
            if (xxx2 < 0 || xxx2 > width) xxx2 = xxx;
            int yyy2 = yyy + UPDATE_TEMPERATURE_CONVECTION_MAXDISTANCE*cells[yyy][yyy].windy/128.0;
            if (yyy2 < 0 || yyy2 > height) yyy2 = yyy;
            cells[xxx][yyy].temperature = oldTemperature[xxx2][yyy2];

            // Conduction
            unsigned int neighbourSum = 0;
            for (auto neighbour : getNeighbours(&cells[xxx][yyy] , UPDATE_TEMPERATURE_CONDUCTION_AREA)) neighbourSum+=neighbour->temperature;
            cells[xxx][yyy].temperature = neighbourSum/getNeighbours(&cells[xxx][yyy] , UPDATE_TEMPERATURE_CONDUCTION_AREA).size;

            // Sunlight
            cells[xxx][yyy].temperature += UPDATE_TEMPERATURE_SUNLIGHT_MULTIPLIER
                                         * cosDecay(cells[xxx][yyy].coverage/255.0 , UPDATE_TEMPERATURE_SUNLIGHT_COVERAGE_POWER) 
                                         * cosCurve(cells[xxx][yyy].y/float(height)-0.5 , UPDATE_TEMPERATURE_SUNLIGHT_LATITUDE_POWER);
            
            // Rain
            cells[xxx][yyy].temperature -= cells[xxx][yyy].temperature * UPDATE_TEMPERATURE_RAIN_MULTIPLIER * cells[xxx][yyy].rain/128.0;
        }
    }
}

void Map::updateHumidity() {

}

void Map::updateWind() {

}

void Map::updateCoverage() {

}

void Map::updateRain() {

}
