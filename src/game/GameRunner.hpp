#pragma once

#include<SFML/Graphics.hpp>
#include<vector>
#include<memory>

#include "entities/Snake.hpp"
#include "entities/GreenCircle.hpp"

class GameRunner
{
private:
    uint8_t exceedBoundary(sf::Vector2f);
    GreenCircle* createGreenCircle(sf::Vector2f);

protected:
    size_t frameNumber;
    float height, width;
    std::vector<std::unique_ptr<GreenCircle>> greenCircles, eatenGreenCircles;
    std::unique_ptr<Snake> snake;
    
public:

    void newGreenCircles(int=1);
    void newSnake(sf::Vector2f, int);
    GameRunner(float, float);
    void step();
};