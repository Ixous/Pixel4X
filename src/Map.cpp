#include "Map.hpp"

Map::Map(unsigned short _width, unsigned short _height, int _seed) :
map(this) , height(_height) , width(_width) , cells(_width,std::vector<Cell>(_height)) , seed(_seed) 
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
}

void Map::update() {
    updateHumidity();
}

void Map::mapmode(Mapmode _mapmode) {
    switch (_mapmode) {
        case Mapmode::HEIGHT:
            selectHeightMap();
            break;
        case Mapmode::TEMPERATURE:
            selectTemperatureMap();
            break;
        case Mapmode::HUMIDITY:
            selectHumidityMap();
            break;
        case Mapmode::DEBUG_CONTINENTSIZE:
            selectContinentSizeMap();
            break;
        case Mapmode::DEBUG_DISTANCETOCOAST:
            selectDistanceToCoastMap();
            break;
        default:
            selectHeightMap();
            break;
    }
}
