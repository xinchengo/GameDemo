#pragma once

#include<vector>
#include<array>
#include<memory>

#include "constants.hpp"
#include "utils.hpp"

class HasCenter
{
protected:
    sf::Vector2f center;
public:
    void setCenter(sf::Vector2f& C)
    {
        center = C;
    }
    sf::Vector2f getCenter()
    {
        return center;
    }
};
class HasVelocity
{
protected:
    sf::Vector2f velocity;
public:
    void setVelocity(sf::Vector2f& C)
    {
        velocity = C;
    }
    void setRandomVelocity(float x)
    {
        float arg = randBetween(-CONST::PI, CONST::PI);
        velocity = rotate(sf::Vector2f(x, 0.0f), arg);
    }
    sf::Vector2f getVelocity()
    {
        return velocity;
    }
};


class SensoryState
{
public:
    std::array<float, CONST::LIDAR_CNT> lidar;
};


class CircularEater : public HasCenter, public HasVelocity
{
private:
    float radius;
    float growthRate;
    sf::Color color;
public:
    CircularEater();
    CircularEater(sf::Vector2f);
    CircularEater(float);
    void step();
    void render(sf::RenderWindow &);
    float getRadius();
    void bounce(uint8_t);
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

class Fish : public HasCenter, public HasVelocity
{
private:
    size_t deathTime;
public:
    SensoryState sensory;
    std::unique_ptr<FishStrategy> strategy;
    Fish(sf::Vector2f, std::unique_ptr<FishStrategy>);
    void step();
    void render(sf::RenderWindow &);
    void die(size_t);
    bool isDead(size_t);
    size_t timeOfDeath();
    void updateVelocity();
};
