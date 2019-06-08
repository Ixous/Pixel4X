#pragma once

class Nation;

#include <vector>

#include "Province.hpp"

class Nation {
public:
    Nation();
    ~Nation();

	std::string name;
	sf::Color color;
    
    Province* capitalProvince;
    std::vector<Province*> provinces;
};