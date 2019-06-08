#pragma once

class City;

#include <vector>

#include "Cell.hpp"
#include "Character.hpp"
#include "Province.hpp"
#include "Population.hpp"

class City {
public:
    City(Cell*);
    ~City();
	
	sf::Color color;

    Cell* location;

	Province* province;

	Character* ruler;
	Population population;

	void assign(Province*);
};