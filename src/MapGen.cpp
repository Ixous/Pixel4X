#include "Map.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <random>
#include <deque>
#include <vector>
#include <algorithm>

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

void Map::calcDistanceToCoast() {
    for (auto coastcell : coast) coastcell->distanceToCoast=0;
    calcDistanceToCoast_iter(coast, 1);
}
void Map::calcDistanceToCoast_iter(std::vector<Cell*> layer, unsigned short distance) {
    std::vector<Cell*> nextlayer;
    for (auto layercell : layer) {
        unsigned short xxx = layercell->x;
        unsigned short yyy = layercell->y;
        if (xxx>=1 && xxx<width-1 && yyy>=1 && yyy<height-1) {
            if (cells[xxx-1][yyy].distanceToCoast==65535 && cells[xxx-1][yyy].height>=0) {cells[xxx-1][yyy].distanceToCoast=distance; nextlayer.push_back(&cells[xxx-1][yyy]);}
            if (cells[xxx][yyy-1].distanceToCoast==65535 && cells[xxx][yyy-1].height>=0) {cells[xxx][yyy-1].distanceToCoast=distance; nextlayer.push_back(&cells[xxx][yyy-1]);}
            if (cells[xxx+1][yyy].distanceToCoast==65535 && cells[xxx+1][yyy].height>=0) {cells[xxx+1][yyy].distanceToCoast=distance; nextlayer.push_back(&cells[xxx+1][yyy]);}
            if (cells[xxx][yyy+1].distanceToCoast==65535 && cells[xxx][yyy+1].height>=0) {cells[xxx][yyy+1].distanceToCoast=distance; nextlayer.push_back(&cells[xxx][yyy+1]);}
        }
    }
    if (nextlayer.size()!=0) calcDistanceToCoast_iter(nextlayer, distance+1);
}

void Map::calcContinentSize() {
    std::deque<Cell*> queue;
    std::vector<Cell*> continent;
    for (unsigned short xxx = 1; xxx < width-1; xxx++) {
        for (unsigned short yyy = 1; yyy < height-1; yyy++) {
            if (cells[xxx][yyy].continentSize==65535) {
                bool land = cells[xxx][yyy].height>=0;
                queue.push_back(&cells[xxx][yyy]);
                continent.push_back(&cells[xxx][yyy]);
                cells[xxx][yyy].continentSize = 0;
                while (!queue.empty()) {
                    unsigned short txxx = queue.back()->x;
                    unsigned short tyyy = queue.back()->y;
                    queue.pop_back();
                    if (txxx >= 1 && txxx<width-1 && tyyy>=1 && tyyy<height-1) {
                        if (cells[txxx-1][tyyy].continentSize==65535 && ((land && cells[txxx-1][tyyy].height>=0) || (!land && cells[txxx-1][tyyy].height<0))) {
                            cells[txxx-1][tyyy].continentSize=0;
                            queue.push_back(&cells[txxx-1][tyyy]);
                            continent.push_back(&cells[txxx-1][tyyy]);
                        }
                        if (cells[txxx][tyyy-1].continentSize==65535 && ((land && cells[txxx][tyyy-1].height>=0) || (!land && cells[txxx][tyyy-1].height<0))) {
                            cells[txxx][tyyy-1].continentSize=0;
                            queue.push_back(&cells[txxx][tyyy-1]);
                            continent.push_back(&cells[txxx][tyyy-1]);
                        }
                        if (cells[txxx+1][tyyy].continentSize==65535 && ((land && cells[txxx+1][tyyy].height>=0) || (!land && cells[txxx+1][tyyy].height<0))) {
                            cells[txxx+1][tyyy].continentSize=0;
                            queue.push_back(&cells[txxx+1][tyyy]);
                            continent.push_back(&cells[txxx+1][tyyy]);
                        }
                        if (cells[txxx][tyyy+1].continentSize==65535 && ((land && cells[txxx][tyyy+1].height>=0) || (!land && cells[txxx][tyyy+1].height<0))) {
                            cells[txxx][tyyy+1].continentSize=0;
                            queue.push_back(&cells[txxx][tyyy+1]);
                            continent.push_back(&cells[txxx][tyyy+1]);
                        }
                    }
                }
                unsigned short continentSize = continent.size();
                for (auto cell : continent) {
                    cell->continentSize = continentSize;
                }
                queue.clear(); //should be useless
                continent.clear();
            }
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
        if (river_seed->continentSize>400 && river_seed->y>130 && river_seed->y<height-130) {
            unsigned short x = river_seed->x;
            unsigned short y = river_seed->y;
            short inertia_x = 0;
            short inertia_y = 0;
            if (cells[x+1][y].height>=0) inertia_x += 40;
            if (cells[x-1][y].height>=0) inertia_x -= 40;
            if (cells[x][y+1].height>=0) inertia_y += 40;
            if (cells[x][y-1].height>=0) inertia_y -= 40;

            if (cells[x-1][y-1].height>=0) {inertia_x -= 15;inertia_y -= 15;}
            if (cells[x+1][y-1].height>=0) {inertia_x += 15;inertia_y -= 15;}
            if (cells[x+1][y+1].height>=0) {inertia_x += 15;inertia_y += 15;}
            if (cells[x-1][y+1].height>=0) {inertia_x -= 15;inertia_y += 15;}
            
            if (cells[x+2][y].height>=0) inertia_x += 5;
            if (cells[x-2][y].height>=0) inertia_x -= 5;
            if (cells[x][y+2].height>=0) inertia_y += 5;
            if (cells[x][y-2].height>=0) inertia_y -= 5;
            spreadRiver(x,y,inertia_x,inertia_y);
            nbr_rivers++;
        }
    }
}

void Map::spreadRiver(unsigned short x, unsigned short y, short inertia_x, short inertia_y) {
    if (x >= 1 && x<width-1 && y>=1 && y<height-1 && cells[x][y].height>=0) {
        cells[x][y].height = -1;
        cells[x][y].river = true;
        cells[x][y].continentSize = 0;
        if (fabs(inertia_x)<1 && fabs(inertia_y)<1) return;
        if (rand()%(int(sqrt(inertia_x*inertia_x + inertia_y*inertia_y)))==0) return;
        unsigned short next_x = x;
        unsigned short next_y = y;
        short next_inertia_x = inertia_x;
        short next_inertia_y = inertia_y;
        if (rand()%int(fabs(std::max(int(inertia_x),1))) >= rand()%int(fabs(std::max(int(inertia_y),1)))) {
            if (inertia_x>0) next_x+=1;
            else if (inertia_x<0) next_x-=1;
        } else {
            if (inertia_y>0) next_y+=1;
            else if (inertia_y<0) next_y-=1;
        }
        if (next_inertia_x>0) next_inertia_x-=10;
        else if (next_inertia_x<0) next_inertia_x+=10;
        if (next_inertia_y>0) next_inertia_y-=10;
        else if (next_inertia_y<0) next_inertia_y+=10;

        auto diffDistx = cells[x+1][y].distanceToCoast - cells[x-1][y].distanceToCoast;
        if (diffDistx>0) next_inertia_x+=diffDistx*10;
        if (diffDistx<0) next_inertia_x+=diffDistx*10;
        auto diffDisty = cells[x][y+1].distanceToCoast - cells[x][y-1].distanceToCoast;
        if (diffDisty>0) next_inertia_y+=diffDisty*10;
        if (diffDisty<0) next_inertia_y+=diffDisty*10;

        auto diffHeightx = cells[x+1][y].height - cells[x-1][y].height;
        if (diffHeightx>0) next_inertia_x+=diffHeightx*5;
        if (diffHeightx<0) next_inertia_x+=diffHeightx*5;
        auto diffHeighty = cells[x][y+1].height - cells[x][y-1].height;
        if (diffHeighty>0) next_inertia_y+=diffHeighty*5;
        if (diffHeighty<0) next_inertia_y+=diffHeighty*5;

        spreadRiver(next_x,next_y,next_inertia_x,next_inertia_y);
    }
}


void Map::genTemperature() {
    unsigned short t_halfHeight = height/2;
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            float noisex = xxx + width*(seed%width);
            float noisey = yyy + width*int(seed/width);
            float noiseMap = 1.5*snoise.fractal(5, noisex,noisey);
            if (yyy <= t_halfHeight) { //northern hemisphere
                cells[xxx][yyy].temperature = -20 + 30*(0.5 + 2.0f*atan((yyy-0.1f*height)/45.0f)/PI) + 15*(0.5 + 2.0f*atan((yyy-0.39f*height)/45.0f)/PI) + 10*noiseMap; //each arctan function create a jump (one between the poles and the temperate zone and one between the temperate zone and the hot ring)
            }
            else { //southern hemisphere
                cells[xxx][yyy].temperature = -20 + 30*(0.5 + 2.0f*atan((height-yyy-0.1f*height)/45.0f)/PI) + 15*(0.5 + 2.0f*atan((height-yyy-0.39f*height)/45.0f)/PI) + 10*noiseMap;
            }
        }
    }
    // now blur the line between the two sides //can be optimized... //TODO : make it linear maybe
    float t_mapWidth = width;
    for (unsigned short yyy = 0; yyy < height; yyy++) {
        // first the left side, first 5%
        for (unsigned short xxx = 0; xxx < t_mapWidth*0.05; xxx++) {
            cells[xxx][yyy].temperature = cells[xxx][yyy].temperature * (0.5+xxx/(t_mapWidth*0.1))
                                           + cells[width -1 - xxx][yyy].temperature * (0.5-xxx/(t_mapWidth*0.1));
        }
        // then the right side, last 5%
        for (unsigned short xxx = t_mapWidth*0.95; xxx < width; xxx++) {
            cells[xxx][yyy].temperature = cells[xxx][yyy].temperature * (1-(xxx-t_mapWidth*0.95)/(t_mapWidth*0.1))
                                           + cells[width -1 - xxx][yyy].temperature * (xxx-t_mapWidth*0.95)/(t_mapWidth*0.1);
        }
    }
}

void Map::genHumidity() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            if (cells[xxx][yyy].height>=0) cells[xxx][yyy].humidity = 50;
            else cells[xxx][yyy].humidity = 100;
        }
    }
}

void Map::genWind() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            cells[xxx][yyy].windx = 0;
            cells[xxx][yyy].windy = 0;
        }
    }
}

void Map::genCloud() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            cells[xxx][yyy].cloud = 0;
        }
    }
}

void Map::genRain() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            cells[xxx][yyy].rain = 0;
        }
    }
}

void Map::genFertility() {
    for (unsigned short xxx = 0; xxx < width; xxx++) {
        for (unsigned short yyy = 0; yyy < height; yyy++) {
            if (cells[xxx][yyy].height>=0) {
                int8_t t = cells[xxx][yyy].temperature;
                double normT; //normal curve
                if (t < 0) normT=0;
                else if (t < 10) normT=2.6*t;
                else if (t < 15) normT=15*t-124;
                else if (t < 20) normT=25*t-274;
                else if (t < 22) normT=13*t-34;
                else if (t < 25) normT=-9*t+450;
                else if (t < 30) normT=-20*t+725;
                else if (t < 40) normT=-6*t+305;
                else if (t < 50) normT=-5*t+265;
                else if (t < 65) normT=-t+65;
                else if (t >= 65) normT=0;

                int8_t h = fmin(cells[xxx][yyy].humidity, cells[xxx][yyy].humidity-15);
                double normH; //normal curve
                if (h < 0) normH=0;
                else if (h < 10) normH=2*h;
                else if (h < 20) normH=6*h-40;
                else if (h < 30) normH=8*h-80;
                else if (h < 40) normH=6*h-20;
                else if (h < 45) normH=4*h+60;
                else if (h < 60) normH=h+195;
                else if (h < 70) normH=375-2*h;
                else if (h < 80) normH=1075-12*h;
                else if (h < 103) normH=515-5*h;
                else if (h >= 103) normH=0;

                cells[xxx][yyy].fertility = log((normT*normH/255.0)/(15.0) +1.0)*255.0/3.0;
            } else cells[xxx][yyy].fertility = 0;
        }
    }
}