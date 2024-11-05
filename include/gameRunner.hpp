#pragma once

#include<SFML/Graphics.hpp>
#include<vector>
#include<string>
#include<memory>

#include "entities.hpp"
#include "utils.hpp"
#include "constants.hpp"

class GameRunner
{
private:
    uint8_t exceedBoundary(sf::Vector2f);
    void updateSensoryState(std::unique_ptr<Fish> &);
    Fish* createFish(sf::Vector2f, CONST::FISH_STRATEGY);
    Fish* createFish(sf::Vector2f, std::unique_ptr<FishStrategy> &);
    CircularEater* createCircularEater(sf::Vector2f);

protected:
    size_t frameNumber;
    float height, width;
    std::vector<std::unique_ptr<CircularEater>> eaters;
    std::vector<std::unique_ptr<Fish>> fishes;
    
    // Fish* createFish(sf::Vector2f, std::unique_ptr<FishStrategy>);
    bool isEaten(sf::Vector2f);
    
public:

    void clear();
    void newRandomFish(CONST::FISH_STRATEGY, int=1);
    void newRandomFish(std::unique_ptr<FishStrategy> &);
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
    std::vector<LinearStrategy> yieldingStrategies;
};

class HeadlessGameRunner : public GameRunner
{
private:
public:
    HeadlessGameRunner(float, float);
    TrainStats train(std::vector<LinearStrategy> &);
};