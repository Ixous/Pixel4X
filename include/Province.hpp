#pragma once

class Province;

#include <vector>

#include "City.hpp"
#include "Cell.hpp"
#include "Character.hpp"

class Province {
public:
    Province();
    ~Province();

	sf::Color color;
    
    City* capitalCity;
    std::vector<City*> cities;
    std::vector<Cell*> cells;

	Character* ruler;
};