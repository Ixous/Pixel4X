#include "City.hpp"

City::City(Cell* _location) :
location(_location)
{
}

City::~City()
{
}

void City::assign(Province* _province) {
    province = _province;
}