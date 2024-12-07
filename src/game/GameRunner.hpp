#pragma once

#include<SFML/Graphics.hpp>
#include<vector>
#include<memory>

#include "entities/Fish.hpp"
#include "entities/Snake.hpp"
#include "entities/GreenCircle.hpp"
#include "entities/CircularEater.hpp"

class GameRunner
{
private:
    uint8_t exceedBoundary(sf::Vector2f);
    void updateSensoryState(std::unique_ptr<Fish> &);
    Fish* createFish(sf::Vector2f, CONST::FISH_STRATEGY);
    Fish* createFish(sf::Vector2f, std::unique_ptr<FishStrategy> &);
    CircularEater* createCircularEater(sf::Vector2f);
    GreenCircle* createGreenCircle(sf::Vector2f);

protected:
    size_t frameNumber;
    float height, width;
    std::vector<std::unique_ptr<CircularEater>> eaters;
    std::vector<std::unique_ptr<Fish>> fishes;
    std::vector<std::unique_ptr<GreenCircle>> greenCircles;
    Snake snake;
    
    // Fish* createFish(sf::Vector2f, std::unique_ptr<FishStrategy>);
    bool isEaten(sf::Vector2f);
    
public:

    void clear();
    void newRandomFish(CONST::FISH_STRATEGY, int=1);
    void newRandomFish(std::unique_ptr<FishStrategy> &);
    void newGreenCircles(int=1);
    GameRunner(float, float);
    void step();
    bool fishAllDead();
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