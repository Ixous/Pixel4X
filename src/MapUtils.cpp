#include "Map.hpp"

bool Map::isValid(int x, int y) {
    return (x>=0 && x<width && y>=0 && y<height);
}

std::vector<Cell*> Map::getNeighbours(Cell* cell, unsigned int distance=1, bool square=false) { // TODO : neighbours across the border of cylinder
    std::vector<Cell*> neighbours;
    for (size_t xxx = cell->x-distance; xxx < cell->x+distance; xxx++) {
        for (size_t yyy = cell->y-distance; yyy < cell->y+distance; yyy++) {
            if (isValid(xxx,yyy) && !(xxx==cell->x || yyy==cell->y)) {
                if (square) neighbours.push_back(&cells[xxx][yyy]);
                else if (xxx+yyy<=distance) neighbours.push_back(&cells[xxx][yyy]);
            }
        }
    }
    return neighbours;
}

std::vector<Cell*> Map::getFertileLands() {
    std::vector<Cell*> fertileLands;
    for (uint16_t xxx = 0; xxx < width; xxx++) {
        for (uint16_t yyy = 0; yyy < height; yyy++) {
            if (cells[xxx][yyy].fertility>80) {
                fertileLands.push_back(&cells[xxx][yyy]);
            }
        }
    }
    return fertileLands;
}
