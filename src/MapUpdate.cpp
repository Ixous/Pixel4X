#include "Map.hpp"

#include "Utils.hpp"

#include <cmath> //sin() , atan() , pow()
#include <stdlib.h> //rand()
#include <iostream>

#include "SimplexNoise.h"
SimplexNoise snoise_weather{.0025f,1.0f,2.2f,0.5f};

#define PI 3.14159265358979323846

#define WEATHER_TEMPERATURE_CONVECTION_MAXDISTANCE 10
#define WEATHER_TEMPERATURE_CONDUCTION_AREA 1
#define WEATHER_TEMPERATURE_SUNLIGHT_MULTIPLIER 2
#define WEATHER_TEMPERATURE_SUNLIGHT_CLOUD_POWER 2
#define WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_POWER 1
#define WEATHER_TEMPERATURE_RAIN_MULTIPLIER 0.4

#define WEATHER_HUMIDITY_CONVECTION_MAXDISTANCE 10
#define WEATHER_HUMIDITY_CONDUCTION_AREA 1
#define WEATHER_HUMIDITY_EVAPORATION_WATER_MULTIPLIER 0.05
#define WEATHER_HUMIDITY_EVAPORATION_LAND_MULTIPLIER 0.05
#define WEATHER_HUMIDITY_RAIN_MULTIPLIER 0.4

#define WEATHER_CLOUD_CONVECTION_MAXDISTANCE 10
#define WEATHER_CLOUD_FORMATION_HUMIDITY_MINIMUM 40
#define WEATHER_CLOUD_FORMATION_TEMPERATURE_MINIMUM 30
#define WEATHER_CLOUD_RAIN_MULTIPLIER 0.4

#define WEATHER_RAIN_CLOUD_MINIMUM 20

void Map::weatherConduction() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            int neighbourTemperatureSum = 0;
            for (auto neighbour : getNeighbours(&cells[xxx][yyy] , WEATHER_TEMPERATURE_CONDUCTION_AREA,false)) neighbourTemperatureSum+=neighbour->temperature;
            cells[xxx][yyy].temperature = neighbourTemperatureSum/getNeighbours(&cells[xxx][yyy] , WEATHER_TEMPERATURE_CONDUCTION_AREA, false).size();
            unsigned int neighbourHumiditySum = 0;
            for (auto neighbour : getNeighbours(&cells[xxx][yyy] , WEATHER_HUMIDITY_CONDUCTION_AREA,false)) neighbourHumiditySum+=neighbour->humidity;
            cells[xxx][yyy].humidity = neighbourHumiditySum/getNeighbours(&cells[xxx][yyy] , WEATHER_HUMIDITY_CONDUCTION_AREA, false).size();
        }
    }
}

void Map::weatherConvection() {
    std::vector<std::vector<unsigned char>> oldTemperature(width, std::vector<unsigned char>(height));
    std::vector<std::vector<unsigned char>> oldHumidity(width, std::vector<unsigned char>(height));
    std::vector<std::vector<unsigned char>> oldCloud(width, std::vector<unsigned char>(height));
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            oldTemperature[xxx][yyy] = cells[xxx][yyy].temperature;
            oldHumidity[xxx][yyy] = cells[xxx][yyy].humidity;
            oldCloud[xxx][yyy] = cells[xxx][yyy].cloud;
        }
    }
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            int xxx2 = xxx + WEATHER_TEMPERATURE_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windx/128.0;
            if (xxx2 < 0 || xxx2 > width) xxx2 = xxx;
            int yyy2 = yyy + WEATHER_TEMPERATURE_CONVECTION_MAXDISTANCE*cells[yyy][yyy].windy/128.0;
            if (yyy2 < 0 || yyy2 > height) yyy2 = yyy;
            cells[xxx][yyy].temperature = oldTemperature[xxx2][yyy2];
            xxx2 = xxx + WEATHER_HUMIDITY_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windx/128.0;
            if (xxx2 < 0 || xxx2 > width) xxx2 = xxx;
            yyy2 = yyy + WEATHER_HUMIDITY_CONVECTION_MAXDISTANCE*cells[yyy][yyy].windy/128.0;
            if (yyy2 < 0 || yyy2 > height) yyy2 = yyy;
            cells[xxx][yyy].humidity = oldHumidity[xxx2][yyy2];
        }
    }
}

void Map::weatherSunlight() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            cells[xxx][yyy].temperature += WEATHER_TEMPERATURE_SUNLIGHT_MULTIPLIER
                                         * cosDecay(cells[xxx][yyy].cloud/255.0 , WEATHER_TEMPERATURE_SUNLIGHT_CLOUD_POWER) 
                                         * cosCurve(cells[xxx][yyy].y/float(height)-0.5 , WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_POWER);
        }
    }
}

void Map::weatherEvaporation() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            if (cells[xxx][yyy].temperature>0) {
                if (cells[xxx][yyy].height<0)
                    cells[xxx][yyy].humidity += WEATHER_HUMIDITY_EVAPORATION_WATER_MULTIPLIER*cells[xxx][yyy].temperature;
                else cells[xxx][yyy].humidity -= WEATHER_HUMIDITY_EVAPORATION_LAND_MULTIPLIER*cells[xxx][yyy].temperature;
                // TODO : temperature decreases with evaporation
            }
        }
    }
}

void Map::weatherRain() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            cells[xxx][yyy].temperature -= cells[xxx][yyy].temperature * WEATHER_TEMPERATURE_RAIN_MULTIPLIER * cells[xxx][yyy].rain/255.0;
            cells[xxx][yyy].humidity += WEATHER_HUMIDITY_RAIN_MULTIPLIER * cells[xxx][yyy].rain/255.0;
            cells[xxx][yyy].cloud -= WEATHER_CLOUD_RAIN_MULTIPLIER * cells[xxx][yyy].rain/255.0;
            if (cells[xxx][yyy].cloud>WEATHER_RAIN_CLOUD_MINIMUM) cells[xxx][yyy].rain = cells[xxx][yyy].cloud - cells[xxx][yyy].temperature;
        }
    }
}

void Map::weatherCloud() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            if (cells[xxx][yyy].humidity>WEATHER_CLOUD_FORMATION_HUMIDITY_MINIMUM && cells[xxx][yyy].humidity>WEATHER_CLOUD_FORMATION_TEMPERATURE_MINIMUM)
                cells[xxx][yyy].cloud += 5; // TODO : put a real formula here...
        }
    }
}

void Map::weatherWind() {
    static float time;
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            cells[xxx][yyy].windx = snoise_weather.fractal(2, time,0);
            cells[xxx][yyy].windy = snoise_weather.fractal(2, 0,time);
        }
    }
}
