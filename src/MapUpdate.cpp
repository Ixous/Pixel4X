#include "Map.hpp"

#include "Utils.hpp"

#include <cmath> //sin() , atan() , pow()
#include <algorithm>
#include <stdlib.h> //rand()

#include <iostream>

#include "SimplexNoise.h"
SimplexNoise snoise_weather{.0025f,1.0f,2.2f,0.5f};

#define PI 3.14159265358979323846

#define WEATHER_TEMPERATURE_CONVECTION_MAXDISTANCE 5
#define WEATHER_TEMPERATURE_SUNLIGHT_MULTIPLIER 10
#define WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_MAIN_POWER 3.0
#define WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_EQUATOR_POWER 0.3
#define WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_EQUATOR_WIDTH 4
#define WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_EQUATOR_RATIO 0.5
#define WEATHER_TEMPERATURE_SUNLIGHT_CLOUD_POWER 2
#define WEATHER_TEMPERATURE_SUNLIGHT_TEMPERATURE_POWER 0.5
#define WEATHER_TEMPERATURE_SUNLIGHT_RADIATION_MULTIPLIER 5
#define WEATHER_TEMPERATURE_SUNLIGHT_RADIATION_POWER 0.2
#define WEATHER_TEMPERATURE_RAIN_MULTIPLIER 0.4

#define WEATHER_HUMIDITY_CONVECTION_MAXDISTANCE 3
#define WEATHER_HUMIDITY_EVAPORATION_SEA_STABILITY_HUMIDITY 30
#define WEATHER_HUMIDITY_EVAPORATION_SEA_STABILITY_RATE 0.5
#define WEATHER_HUMIDITY_EVAPORATION_SEA_TEMPERATURE_MINIMUM -20.0
#define WEATHER_HUMIDITY_EVAPORATION_SEA_RATE 0.1
#define WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_HUMIDITY 25
#define WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_TEMPERATURE_MINIMUM 14.0
#define WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_TEMPERATURE_POWER 1.0
#define WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_RATE 0.4
#define WEATHER_HUMIDITY_RAIN_MULTIPLIER 0.4

#define WEATHER_CLOUD_CONVECTION_MAXDISTANCE 5
#define WEATHER_CLOUD_FORMATION_HUMIDITY_MINIMUM 40
#define WEATHER_CLOUD_FORMATION_TEMPERATURE_MINIMUM 30
#define WEATHER_CLOUD_RAIN_MULTIPLIER 0.4

#define WEATHER_RAIN_CLOUD_MINIMUM 20

void Map::weatherConduction() {
    std::vector<std::vector<char>> oldTemperature(width, std::vector<char>(height));
    std::vector<std::vector<unsigned char>> oldHumidity(width, std::vector<unsigned char>(height));
    std::vector<std::vector<unsigned char>> oldCloud(width, std::vector<unsigned char>(height));
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            oldTemperature[xxx][yyy] = cells[xxx][yyy].temperature;
            oldHumidity[xxx][yyy] = cells[xxx][yyy].humidity;
            oldCloud[xxx][yyy] = cells[xxx][yyy].cloud;
        }
    }
    for (unsigned short yyy = 1; yyy < height-1; yyy++) {
        int neighbourTemperatureSum = 0;
        neighbourTemperatureSum += oldTemperature[width-1][yyy];
        neighbourTemperatureSum += oldTemperature[1][yyy];
        neighbourTemperatureSum += oldTemperature[0][yyy-1];
        neighbourTemperatureSum += oldTemperature[0][yyy+1];
        cells[0][yyy].temperature = neighbourTemperatureSum/4;
        
        int neighbourHumiditySum = 0;
        neighbourHumiditySum += oldHumidity[width-1][yyy];
        neighbourHumiditySum += oldHumidity[1][yyy];
        neighbourHumiditySum += oldHumidity[0][yyy-1];
        neighbourHumiditySum += oldHumidity[0][yyy+1];
        cells[0][yyy].humidity = neighbourHumiditySum/4;
    }
    for (unsigned short xxx = 1; xxx < width-1; xxx++) {
        for (unsigned short yyy = 1; yyy < height-1; yyy++) {
            int neighbourTemperatureSum = 0;
            neighbourTemperatureSum += oldTemperature[xxx-1][yyy];
            neighbourTemperatureSum += oldTemperature[xxx+1][yyy];
            neighbourTemperatureSum += oldTemperature[xxx][yyy-1];
            neighbourTemperatureSum += oldTemperature[xxx][yyy+1];
            cells[xxx][yyy].temperature = neighbourTemperatureSum/4;

            int neighbourHumiditySum = 0;
            neighbourHumiditySum += oldHumidity[xxx-1][yyy];
            neighbourHumiditySum += oldHumidity[xxx+1][yyy];
            neighbourHumiditySum += oldHumidity[xxx][yyy-1];
            neighbourHumiditySum += oldHumidity[xxx][yyy+1];
            cells[xxx][yyy].humidity = neighbourHumiditySum/4;
        }
    }
    for (unsigned short yyy = 1; yyy < height-1; yyy++) {
        int neighbourTemperatureSum = 0;
        neighbourTemperatureSum += oldTemperature[width-2][yyy];
        neighbourTemperatureSum += oldTemperature[0][yyy];
        neighbourTemperatureSum += oldTemperature[width-1][yyy-1];
        neighbourTemperatureSum += oldTemperature[width-1][yyy+1];
        cells[width-1][yyy].temperature = neighbourTemperatureSum/4;
        
        int neighbourHumiditySum = 0;
        neighbourHumiditySum += oldHumidity[width-2][yyy];
        neighbourHumiditySum += oldHumidity[0][yyy];
        neighbourHumiditySum += oldHumidity[width-1][yyy-1];
        neighbourHumiditySum += oldHumidity[width-1][yyy+1];
        cells[width-1][yyy].humidity = neighbourHumiditySum/4;
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
    float t_cosCurve;
    for (unsigned short yyy = 0; yyy < height; yyy++) {
        t_cosCurve = 1.6*(cosCurve(yyy/float(height-2)-0.5 , 1)-0.4);
        for (unsigned short xxx = 0; xxx < width; xxx++) {
            int xxx2 = xxx + WEATHER_TEMPERATURE_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windx/128.0;
            if (xxx2<0) xxx2 = width+xxx2;
            if (xxx2>=width) xxx2 = xxx2%width;
            int yyy2 = yyy + t_cosCurve*WEATHER_TEMPERATURE_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windy/128.0;
            if (yyy2 < 0 || yyy2 >= height) yyy2 = yyy;
            cells[xxx][yyy].temperature = oldTemperature[xxx2][yyy2];

            xxx2 = xxx + WEATHER_HUMIDITY_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windx/128.0;
            if (xxx2<0) xxx2 = width+xxx2;
            if (xxx2>=width) xxx2 = xxx2%width;
            yyy2 = yyy + t_cosCurve*WEATHER_HUMIDITY_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windy/128.0;
            if (yyy2 < 0 || yyy2 >= height) yyy2 = yyy;
            cells[xxx][yyy].humidity = oldHumidity[xxx2][yyy2];

            xxx2 = xxx + WEATHER_CLOUD_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windx/128.0;
            if (xxx2<0) xxx2 = width+xxx2;
            if (xxx2>=width) xxx2 = xxx2%width;
            yyy2 = yyy + t_cosCurve*WEATHER_CLOUD_CONVECTION_MAXDISTANCE*cells[xxx][yyy].windy/128.0;
            if (yyy2 < 0 || yyy2 >= height) yyy2 = yyy;
            cells[xxx][yyy].cloud = oldCloud[xxx2][yyy2];
        }
    }
}

void Map::weatherSunlight() {
    float t_cosCurve;
    for (unsigned short yyy = 0; yyy < height; yyy++) {
        t_cosCurve = (1-WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_EQUATOR_RATIO)*cosCurve(2*yyy/float(height-2)-1.0 , WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_MAIN_POWER)
                    +WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_EQUATOR_RATIO*cosCurve(WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_EQUATOR_WIDTH*(yyy/float(height-2)-0.5) , WEATHER_TEMPERATURE_SUNLIGHT_LATITUDE_EQUATOR_POWER);
        for (unsigned short xxx = 0; xxx < width; xxx++) {
            auto temp = WEATHER_TEMPERATURE_SUNLIGHT_MULTIPLIER
                        * t_cosCurve 
                        * cosDecay(cells[xxx][yyy].cloud/255.0, WEATHER_TEMPERATURE_SUNLIGHT_CLOUD_POWER)
                        * cosDecay(cells[xxx][yyy].temperature/128.0, WEATHER_TEMPERATURE_SUNLIGHT_TEMPERATURE_POWER);
            cells[xxx][yyy].temperature += fmax(0,temp);
            cells[xxx][yyy].temperature = fmin(cells[xxx][yyy].temperature,(char)(cells[xxx][yyy].temperature-WEATHER_TEMPERATURE_SUNLIGHT_RADIATION_MULTIPLIER * cosDecay(0.5-(cells[xxx][yyy].temperature/255) , WEATHER_TEMPERATURE_SUNLIGHT_RADIATION_POWER)));
        }
    }
}

void Map::weatherEvaporation() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            if (cells[xxx][yyy].height<0) {
                if (cells[xxx][yyy].humidity<WEATHER_HUMIDITY_EVAPORATION_SEA_STABILITY_HUMIDITY) cells[xxx][yyy].humidity += WEATHER_HUMIDITY_EVAPORATION_SEA_STABILITY_RATE*(WEATHER_HUMIDITY_EVAPORATION_SEA_STABILITY_HUMIDITY-cells[xxx][yyy].humidity);
                if (cells[xxx][yyy].temperature>WEATHER_HUMIDITY_EVAPORATION_SEA_TEMPERATURE_MINIMUM)
                    cells[xxx][yyy].humidity += WEATHER_HUMIDITY_EVAPORATION_SEA_RATE*(100-cells[xxx][yyy].humidity)*((cells[xxx][yyy].temperature-WEATHER_HUMIDITY_EVAPORATION_SEA_TEMPERATURE_MINIMUM)/(127.0-WEATHER_HUMIDITY_EVAPORATION_SEA_TEMPERATURE_MINIMUM));
            } else {
                float stabilityPoint = WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_HUMIDITY;
                if (cells[xxx][yyy].temperature > WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_TEMPERATURE_MINIMUM)
                    stabilityPoint *= cosDecay((cells[xxx][yyy].temperature-WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_TEMPERATURE_MINIMUM)/(128.0-WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_TEMPERATURE_MINIMUM),WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_TEMPERATURE_POWER);
                if (cells[xxx][yyy].humidity>stabilityPoint)
                    cells[xxx][yyy].humidity -= WEATHER_HUMIDITY_EVAPORATION_LAND_STABILITY_RATE*(cells[xxx][yyy].humidity-stabilityPoint);
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
            if (cells[xxx][yyy].humidity>WEATHER_CLOUD_FORMATION_HUMIDITY_MINIMUM && cells[xxx][yyy].temperature>WEATHER_CLOUD_FORMATION_TEMPERATURE_MINIMUM)
                cells[xxx][yyy].cloud += 5; // TODO : put a real formula here...
        }
    }
}

void Map::weatherWind() {
    static float time;
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            cells[xxx][yyy].windx = (127.0*(0.2+0.8*cosDecay(cells[xxx][yyy].height/128.0,4)))*(snoise_weather.fractal(2, int(xxx+time)%1000000,yyy) + snoise_weather.fractal(2, int(1000+xxx-time)%1000000,1000+yyy))/2.0;
            cells[xxx][yyy].windy = (127.0*(0.2+0.8*cosDecay(cells[xxx][yyy].height/128.0,4)))*(snoise_weather.fractal(2, 1000+xxx,int(yyy-time)%1000000) + snoise_weather.fractal(2, xxx,int(1000+yyy+time)%1000000))/2.0;
        }
    }
    time+=8;
}


void Map::debug_probetemp() {
    std::cout << "Debug record" << std::endl;
    for (unsigned short yyy = 0; yyy < height/10; yyy++) {
        std::cout << (int)(cells[400][10*yyy].temperature) << std::endl;
    }
}