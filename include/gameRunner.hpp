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
    void clear();
    void createRandomFish(int);
    GameRunner(float, float);
    void step();
    bool fishAllDead();
};

class RenderedGameRunner : public GameRunner
{
private:
    sf::RenderWindow &window;
public:
    void render();
    RenderedGameRunner(sf::RenderWindow &);
};

struct TrainStats
{
    float meanLifespan;
    std::vector<float> lifespans;
    std::vector<FishStrategy> yieldingStrategies;
};

class EvolutionGameRunner : public GameRunner
{
private:
public:
    EvolutionGameRunner(float, float);
    TrainStats train(std::vector<FishStrategy> &);
};