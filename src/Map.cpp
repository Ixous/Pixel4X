#include "Map.hpp"

#include "Game.hpp"

#include <iostream>

Map::Map(unsigned short _width, unsigned short _height, int _seed) :
height(_height) , width(_width) , seed(_seed)  , cells(_width,std::vector<Cell>(_height))
{
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            cells[xxx][yyy].x = xxx;
            cells[xxx][yyy].y = yyy;
        }
    }
}

Map::~Map() {
}


void Map::generate() {
    genHeightMap();
    markCoast();
    calcDistanceToCoast();
    calcContinentSize();
    genRivers();
    genTemperature();
    genHumidity();
    genWind();
    genCloud();
    genRain();
    genFertility();
}

void Map::update() {
    std::cout << "weatherConduction" << std::endl;
    weatherConduction();
    std::cout << "weatherConvection" << std::endl;
    weatherConvection();
    std::cout << "weatherSunlight" << std::endl;
    weatherSunlight();
    std::cout << "weatherEvaporation" << std::endl;
    weatherEvaporation();
    std::cout << "weatherRain" << std::endl;
    // weatherRain();
    std::cout << "weatherCloud" << std::endl;
    // weatherCloud();
    std::cout << "weatherWind" << std::endl;
    weatherWind();
    mapmode(selectedMapmode);
}

void Map::mapmode(Mapmode _mapmode) {
    if (mapimage.getSize() != sf::Vector2u{width,height})
        mapimage.create(width,height);
    switch (_mapmode) {
        case Mapmode::HEIGHT:
            mapmodeHeight();
            break;
        case Mapmode::TEMPERATURE:
            mapmodeTemperature();
            break;
        case Mapmode::HUMIDITY:
            mapmodeHumidity();
            break;
        case Mapmode::WIND:
            mapmodeWind();
            break;
        case Mapmode::CLOUD:
            mapmodeCloud();
            break;
        case Mapmode::RAIN:
            mapmodeRain();
            break;
        case Mapmode::FERTILITY:
            mapmodeFertility();
            break;
        case Mapmode::DEBUG_CONTINENTSIZE:
            mapmodeContinentSize();
            break;
        case Mapmode::DEBUG_DISTANCETOCOAST:
            mapmodeDistanceToCoast();
            break;
        default:
            mapmodeHeight();
            break;
    }
    selectedMapmode = _mapmode;
    maptexture.loadFromImage(mapimage);
    mapsprite.setTexture(maptexture);
}
