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
            sf::Uint8 tempColor = (sf::Uint8)(200-0.5*(fabs(temperature))) - greyWater;
            if (temperature >= 0) {
                mapimage.setPixel(xxx, yyy, sf::Color{(sf::Uint8)(200-greyWater),tempColor,tempColor,255});
            }
            else {
                mapimage.setPixel(xxx, yyy, sf::Color{tempColor,tempColor,(sf::Uint8)(200-greyWater),255});
            }
        }
    }
}

void Map::mapmodeHumidity() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            auto humidity = cells[xxx][yyy].humidity;
            sf::Uint8 greyWater{cells[xxx][yyy].height >= 0 ? (sf::Uint8)0 : (sf::Uint8)20};
            if (humidity<20) {
                mapimage.setPixel(xxx, yyy, sf::Color{200-greyWater,200-20+humidity-greyWater,200-20+humidity-greyWater,255});
            } else {
                mapimage.setPixel(xxx, yyy, sf::Color{200+20-humidity-greyWater,200+20-humidity-greyWater,200-greyWater,255});
            }
        }
    }
}

void Map::mapmodeWind() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            // auto windforce = sqrt(pow(cells[xxx][yyy].windx,2)+pow(cells[xxx][yyy].windy,2)); //TODO : make it work.....
            mapimage.setPixel(xxx, yyy, sf::Color{(sf::Uint8)(128+cells[xxx][yyy].windx),128,(sf::Uint8)(128+cells[xxx][yyy].windy),255});
        }
    }
}

void Map::mapmodeCloud() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            auto cloud = cells[xxx][yyy].cloud;
            mapimage.setPixel(xxx, yyy, sf::Color{200-cloud,200-cloud,200-cloud,255});
        }
    }
}

void Map::mapmodeRain() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            auto rain = cells[xxx][yyy].rain;
            mapimage.setPixel(xxx, yyy, sf::Color{200-rain,200-rain,200,255});
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
