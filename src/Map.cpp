#include "Map.hpp"

Map::Map(unsigned short _width, unsigned short _height, int _seed) :
map(this) , height(_height) , width(_width) , seed(_seed)  , cells(_width,std::vector<Cell>(_height))
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
    genFertility();
}

void Map::update() {
    weatherConduction();
    weatherConvection();
    weatherSunlight();
    weatherEvaporation();
    weatherRain();
    weatherCloud();
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
