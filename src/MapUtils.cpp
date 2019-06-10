#include "Map.hpp"

bool Map::isValid(int x, int y) {
    return (x>=0 && x<width && y>=0 && y<height);
}

std::vector<Cell*> Map::getNeighbours(Cell* cell, unsigned int distance, bool square) { // TODO : neighbours across the border of cylinder
    std::vector<Cell*> neighbours;
    for (int xxx = cell->x-distance; xxx <= int(cell->x+distance); xxx++) {
        for (int yyy = cell->y-distance; yyy <= int(cell->y+distance); yyy++) {
            if (isValid(xxx,yyy) && !(xxx==cell->x && yyy==cell->y)) {
                if (square) neighbours.push_back(&cells[xxx][yyy]);
                else if (fabs(xxx-cell->x)+fabs(yyy-cell->y)<=distance) neighbours.push_back(&cells[xxx][yyy]);
            }
        }
    }
    return neighbours;
}

std::vector<Cell*> Map::getFertileLands() {
    std::vector<Cell*> fertileLands;
    for (uint16_t xxx = 0; xxx < width; xxx++) {
        for (uint16_t yyy = 0; yyy < height; yyy++) {
            auto distanceFromEquator = fabs(yyy-height/2);
            if (distanceFromEquator<height*0.2 && cells[xxx][yyy].distanceToCoast<5) fertileLands.push_back(&cells[xxx][yyy]);
            else if (distanceFromEquator>height*0.2 && distanceFromEquator<height*0.3 && cells[xxx][yyy].distanceToCoast<10) fertileLands.push_back(&cells[xxx][yyy]);
            else if (distanceFromEquator>height*0.3 && distanceFromEquator<height*0.4 && cells[xxx][yyy].distanceToCoast<25) fertileLands.push_back(&cells[xxx][yyy]);
            else if (distanceFromEquator>height*0.4 && distanceFromEquator<height*0.45 && cells[xxx][yyy].distanceToCoast<15) fertileLands.push_back(&cells[xxx][yyy]);
        }
    }
    return fertileLands;
}
