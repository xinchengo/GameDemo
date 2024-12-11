#pragma once

#include<array>

#include "utilities/mathUtils.hpp"
#include "utilities/config.hpp"

class SensoryState
{
public:
    std::array<float, CONST::LIDAR_CNT> lidar;
};

class FishStrategy
{
public:
    virtual sf::Vector2f predictVelocity(SensoryState &, sf::Vector2f) = 0;
};

class LinearStrategy : public FishStrategy
{
private:
    std::array<float, CONST::LIDAR_CNT> a, b;
    float c;
    float acceleration_bias;
public:
    LinearStrategy();
    void mutate();
    sf::Vector2f predictVelocity(SensoryState &sense, sf::Vector2f velocity);
};

class BaselineStrategy : public FishStrategy
{
private:
public:
    sf::Vector2f predictVelocity(SensoryState &, sf::Vector2f);
};