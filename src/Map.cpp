#include "Map.hpp"

Map::Map(unsigned short _width, unsigned short _height, int _seed) :
height(_height) , width(_width) , cells(_width,std::vector<Cell>(_height)) , seed(_seed) 
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
