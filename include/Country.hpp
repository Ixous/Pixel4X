#pragma once

class Country;

#include <vector>

#include "Nation.hpp"

class Country {
public:
    Country();
    ~Country();

	std::string name;
	sf::Color color;

    Nation* capitalNation;
    std::vector<Nation*> nations;
};