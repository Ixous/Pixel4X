#include "Map.hpp"
#define PI 3.14159265358979323846

void Map::draw(sf::RenderWindow* window) {
    window->draw(mapsprite);
}

void Map::selectHeightMap() {
    mapimage.create(width,height);
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            sf::Color color;
            auto cell_height = cells[xxx][yyy].height;
            if (cell_height<0) color = sf::Color{(sf::Uint8)(120+cell_height/3.0),(sf::Uint8)(150+cell_height/3.0),210,255};
            else {
                auto something = (sf::Uint8)(2*cell_height);
                color = sf::Color{something,something,something,255};
            }
            mapimage.setPixel(xxx,yyy, color);
        }
    }
    maptexture.loadFromImage(mapimage);
    mapsprite.setTexture(maptexture);
}

void Map::selectDistanceToCoastMap() {
    mapimage.create(width,height);
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            sf::Color color;
            auto distanceToCoast = cells[xxx][yyy].distanceToCoast;
            auto something = (sf::Uint8)(2*distanceToCoast);            
            color = sf::Color{something,0,0,255};
            mapimage.setPixel(xxx,yyy, color);
        }
    }
    maptexture.loadFromImage(mapimage);
    mapsprite.setTexture(maptexture);
}