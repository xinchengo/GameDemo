#pragma once

#include<SFML/Graphics.hpp>
#include<vector>
#include<memory>
#include<chrono>

#include "entities/Snake.hpp"
#include "entities/GreenCircle.hpp"
#include "entities/boids/Swarm.hpp"

class GameRunner : virtual public HasTimer
{
private:
    uint8_t exceedBoundary(sf::Vector2f pos);
    GreenCircle* createGreenCircle(sf::Vector2f pos);

protected:
    float height, width;
    Swarm fish;
    std::vector<std::unique_ptr<GreenCircle>> greenCircles, eatenGreenCircles;
    std::unique_ptr<Snake> snake;
    
public:

    void newFish(int cnt=1);
    void newGreenCircles(int cnt=1);
    void newSnake(sf::Vector2f position, int length);
    GameRunner(float width, float height);
    void step();
};