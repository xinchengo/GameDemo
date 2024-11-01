#pragma once

#include<SFML/Graphics.hpp>
#include<vector>

#include "entities.hpp"
#include "utils.hpp"

class GameRunner
{
protected:
    size_t frameNumber;
    float height, width;
    std::vector<CircularEater> eaters;
    std::vector<Fish> allTheFish;
    
    bool isEaten(sf::Vector2f);
    bool exceedBoundary(sf::Vector2f);
    void updateSensoryState();

public:
    void createRandomFish(int);
    GameRunner(float, float);
    void step();
};

class RenderedGameRunner : public GameRunner
{
private:
    sf::RenderWindow &window;
public:
    void render();
    RenderedGameRunner(sf::RenderWindow &);
};