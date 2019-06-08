#include "Map.hpp"
#define PI 3.14159265358979323846

void Map::draw(sf::RenderWindow* window) {
    window->draw(mapsprite);
}

void Map::mapmodeHeight() {
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
}

void Map::mapmodeTemperature() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            auto temperature = cells[xxx][yyy].temperature;
            sf::Uint8 greyWater{cells[xxx][yyy].height >= 0 ? (sf::Uint8)0 : (sf::Uint8)20};
            if (temperature >= 15) {
                sf::Uint8 tempColor = (sf::Uint8)(200-2*(temperature-15)) - greyWater;
                mapimage.setPixel(xxx, yyy, sf::Color{(sf::Uint8)(200-greyWater),tempColor,tempColor,255}); //can overflow if temperature is too low or too high
            }
            else {
                sf::Uint8 tempColor = (sf::Uint8)(200+2*(temperature-15)) - greyWater;
                mapimage.setPixel(xxx, yyy, sf::Color{tempColor,tempColor,(sf::Uint8)(200-greyWater),255}); //can overflow if temperature is too low or too high
            }
        }
    }
}

void Map::mapmodeHumidity() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            auto humidity = cells[xxx][yyy].humidity;
            sf::Uint8 greyWater{cells[xxx][yyy].height >= 0 ? (sf::Uint8)0 : (sf::Uint8)20};
            if (humidity >= 15) {
                sf::Uint8 tempColor = (sf::Uint8)(200 - 2*(humidity-40)) - greyWater;
                mapimage.setPixel(xxx, yyy, sf::Color{tempColor,tempColor,(sf::Uint8)(200-greyWater),255});
            }
            else {
                sf::Uint8 tempColor = (sf::Uint8)(200 + 3*(humidity-40)) - greyWater;
                mapimage.setPixel(xxx, yyy, sf::Color{(sf::Uint8)(200-greyWater),tempColor,tempColor,255});
            }
        }
    }
}

void Map::mapmodeFertility() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            uint8_t fertility = cells[xxx][yyy].fertility;
            int8_t t_height = cells[xxx][yyy].height;
            if (t_height<0) mapimage.setPixel(xxx, yyy, sf::Color{(sf::Uint8)(120+t_height/3.0),(sf::Uint8)(120+t_height/3.0),(sf::Uint8)(120+t_height/3.0),255});
            else {
                if (fertility >= 80) {
                    sf::Uint8 tempColor = (sf::Uint8)(200 - (fertility-80)/4);
                    mapimage.setPixel(xxx, yyy, sf::Color{tempColor,(sf::Uint8)(200),tempColor,255});
                }
                else {
                    sf::Uint8 tempColor = (sf::Uint8)(200 + (fertility-80));
                    mapimage.setPixel(xxx, yyy, sf::Color{(sf::Uint8)(200),tempColor,tempColor,255});
                }
            }
        }
    }
}

void Map::mapmodeDistanceToCoast() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            sf::Color color;
            auto distanceToCoast = cells[xxx][yyy].distanceToCoast;
            auto something = (sf::Uint8)(2*distanceToCoast);            
            color = sf::Color{something,0,0,255};
            mapimage.setPixel(xxx,yyy, color);
        }
    }
    for (auto coastcell : coast) mapimage.setPixel(coastcell->x,coastcell->y,sf::Color::Blue);
}

void Map::mapmodeContinentSize() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            sf::Color color;
            auto continentSize = cells[xxx][yyy].continentSize;
            color = sf::Color{0,(sf::Uint8)(255*continentSize/32000.0f),0,255};
            if (cells[xxx][yyy].height<0) color = sf::Color{0,0,(sf::Uint8)(255*continentSize/65000.0f), 255};
            mapimage.setPixel(xxx,yyy, color);
        }
    }
}
