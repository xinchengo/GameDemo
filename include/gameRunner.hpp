#pragma once

#include<SFML/Graphics.hpp>
#include<vector>

#include "entities.hpp"
#include "utils.hpp"


class GameRunner
{
private:
    sf::RenderWindow &window;
    int frameNumber;
    float height, width;
    std::vector<CircularEater> eaters;
    std::vector<Fish> allTheFish;
    
    bool isEaten(sf::Vector2f);

public:
    GameRunner(sf::RenderWindow &);
    void step();
    void render();
};