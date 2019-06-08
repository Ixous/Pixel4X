#include "Game.hpp"

#include <random>

#include "Country.hpp"

void Game::spawnCountries(unsigned short nbrNations) {
    auto fertileLands = map.getFertileLands();
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,fertileLands.size());
    while (nations.size()<nbrNations) {
        Cell* capitalLocation = fertileLands[dist(rng)];
        City city(capitalLocation);
        Province province();
        Country country();
        Nation nation();
    }
}