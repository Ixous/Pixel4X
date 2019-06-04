#include "Map.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <random>

#define PI 3.14159265358979323846

#define MAINNOISE_NOISE_MULTIPLIER 100
#define MAINNOISE_NOISE_OCTAVES 6
#define MAINNOISE_OFFSET 20
#define MAINNOISE_ABSNOISE_MULTIPLIER 80
#define MAINNOISE_ABSNOISE_OCTAVES 5

#define POLES_TEMPER_DISTANCE 0.22
#define POLES_TEMPER_AMPLITUDE 0.5
#define POLES_TEMPER_OFFSET 40
#define SIDES_TEMPER_DISTANCE 0.22
#define SIDES_TEMPER_AMPLITUDE 0.5
#define SIDES_TEMPER_OFFSET 40

#define LAND_HEIGHT_MULTIPLIER 0.4
#define MOUNTAINS_MASK_FREQUENCY 150.0
#define MOUNTAINS_NOISE_OCTAVES 8
#define MOUNTAINS_NOISE_POWER 3
#define MOUNTAINS_SLOPE_DISTANCE 25.0
#define MOUNTAINS_SLOPE_POWER 3
#define MOUNTAINS_MULTIPLIER 100

#include "SimplexNoise.h"
SimplexNoise snoise{.0025f,1.0f,2.2f,0.5f};

void Map::genHeightMap() {
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            size_t noisex = xxx + width*(seed%width);
            size_t noisey = yyy + width*int(seed/width);
            cells[xxx][yyy].height = MAINNOISE_NOISE_MULTIPLIER*snoise.fractal(MAINNOISE_NOISE_OCTAVES, noisex,noisey);
            cells[xxx][yyy].height += MAINNOISE_OFFSET;
            cells[xxx][yyy].height -= MAINNOISE_ABSNOISE_MULTIPLIER*std::abs(snoise.fractal(MAINNOISE_ABSNOISE_OCTAVES, noisex+width,noisey));
            if (yyy < POLES_TEMPER_DISTANCE*height) {
                cells[xxx][yyy].height *= (1-POLES_TEMPER_AMPLITUDE) + POLES_TEMPER_AMPLITUDE*sin(yyy*(PI/2.0f)/(POLES_TEMPER_DISTANCE*height));
                cells[xxx][yyy].height -= POLES_TEMPER_OFFSET*cos(yyy*(PI/2.0f)/(POLES_TEMPER_DISTANCE*height));
            }
            if (yyy > (1-POLES_TEMPER_DISTANCE)*height) {
                cells[xxx][yyy].height *= (1-POLES_TEMPER_AMPLITUDE) + POLES_TEMPER_AMPLITUDE*sin((height-yyy)*(PI/2.0f)/(POLES_TEMPER_DISTANCE*height));
                cells[xxx][yyy].height -= POLES_TEMPER_OFFSET*cos((height-yyy)*(PI/2.0f)/(POLES_TEMPER_DISTANCE*height));
            }
            if (xxx < SIDES_TEMPER_DISTANCE*width) {
                cells[xxx][yyy].height *= (1-SIDES_TEMPER_AMPLITUDE) + SIDES_TEMPER_AMPLITUDE*sin(xxx*(PI/2.0f)/(SIDES_TEMPER_DISTANCE*width));
                cells[xxx][yyy].height -= SIDES_TEMPER_OFFSET*cos(xxx*(PI/2.0f)/(SIDES_TEMPER_DISTANCE*width));
            }
            if (xxx > (1-SIDES_TEMPER_DISTANCE)*width) {
                cells[xxx][yyy].height *= (1-SIDES_TEMPER_AMPLITUDE) + SIDES_TEMPER_AMPLITUDE*sin((width-xxx)*(PI/2.0f)/(SIDES_TEMPER_DISTANCE*width));
                cells[xxx][yyy].height -= SIDES_TEMPER_OFFSET*cos((width-xxx)*(PI/2.0f)/(SIDES_TEMPER_DISTANCE*width));
            }
        }
    }
    for (size_t xxx = 0; xxx < width; xxx++) {
        for (size_t yyy = 0; yyy < height; yyy++) {
            size_t noisex = xxx + width*(seed%width);
            size_t noisey = yyy + width*int(seed/width);
            if (cells[xxx][yyy].height > 0) {
                cells[xxx][yyy].height *= LAND_HEIGHT_MULTIPLIER;
                // cells[xxx][yyy].height += 100*fmax(0,(1-exp(-cells[xxx][yyy].height/25.0)))
                float mask = 0.5+0.5*snoise.noise(noisex/MOUNTAINS_MASK_FREQUENCY,  noisey/MOUNTAINS_MASK_FREQUENCY)  ;
                float noise = pow((1-std::abs(snoise.fractal(MOUNTAINS_NOISE_OCTAVES, noisex+width,noisey))), MOUNTAINS_NOISE_POWER);
                float slope = 1.0;
                if (cells[xxx][yyy].height<MOUNTAINS_SLOPE_DISTANCE) slope = sin(pow(cells[xxx][yyy].height/MOUNTAINS_SLOPE_DISTANCE, MOUNTAINS_SLOPE_POWER)*PI/2.0);
                cells[xxx][yyy].height += MOUNTAINS_MULTIPLIER*slope*noise*mask;
            }
        }
    }
}

void Map::calcDistanceToCoast() {
    calcDistanceToCoast_iter(coast, 0);
}
void Map::calcDistanceToCoast_iter(std::vector<Cell*> layer, unsigned short distance) {
    std::vector<Cell*> nextlayer;
    for (auto layercell : layer) {
        unsigned short xxx = layercell->x;
        unsigned short yyy = layercell->y;
        if (xxx>=1 && xxx<width-1 && yyy>=1 && yyy<height-1) {
            if (cells[xxx-1][yyy].distanceToCoast==32000 && cells[xxx-1][yyy].height>0) {cells[xxx-1][yyy].distanceToCoast=distance; nextlayer.push_back(&cells[xxx-1][yyy]);}
            if (cells[xxx][yyy-1].distanceToCoast==32000 && cells[xxx][yyy-1].height>0) {cells[xxx][yyy-1].distanceToCoast=distance; nextlayer.push_back(&cells[xxx][yyy-1]);}
            if (cells[xxx+1][yyy].distanceToCoast==32000 && cells[xxx+1][yyy].height>0) {cells[xxx+1][yyy].distanceToCoast=distance; nextlayer.push_back(&cells[xxx+1][yyy]);}
            if (cells[xxx][yyy+1].distanceToCoast==32000 && cells[xxx][yyy+1].height>0) {cells[xxx][yyy+1].distanceToCoast=distance; nextlayer.push_back(&cells[xxx][yyy+1]);}
        }
    }
    if (nextlayer.size()!=0) calcDistanceToCoast_iter(nextlayer, distance+1);
}

void Map::calcContinentSize() {
    
}

void Map::markCoast() {
    for (unsigned short xxx = 1; xxx < width-1; xxx++) {
        for (unsigned short yyy = 1; yyy < height-1; yyy++) {
            if (cells[xxx][yyy].height>=0 && (   cells[xxx-1][yyy].height<0 
                                              || cells[xxx][yyy-1].height<0
                                              || cells[xxx+1][yyy].height<0
                                              || cells[xxx][yyy+1].height<0))
                coast.push_back(&cells[xxx][yyy]);
        }
    }
}

void Map::genRivers() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,coast.size());
    int nbr_rivers = 0;
    while (nbr_rivers < 20) {
        Cell* river_seed = coast[dist(rng)];
        spreadRiver(river_seed->x, river_seed->y);
        nbr_rivers++;
    }
}

void Map::spreadRiver(unsigned short x, unsigned short y) {
    unsigned short dirx = x+1, diry=y;
    if (cells[x][y-1].height > cells[dirx][diry].height) { dirx=x; diry=y-1; }
    if (cells[x][y+1].height > cells[dirx][diry].height) { dirx=x; diry=y+1; }
    if (cells[x-1][y].height > cells[dirx][diry].height) { dirx=x-1; diry=y; }
    if (cells[dirx][diry].height >= cells[x][y].height && rand()%500!=0) {
        cells[x][y].height = -1;
        mapimage.setPixel(x, y, sf::Color::Green);    
        maptexture.loadFromImage(mapimage);
        mapsprite.setTexture(maptexture);    
        spreadRiver(dirx,diry);
    }
}
