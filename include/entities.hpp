#pragma once

#include<array>
#include<vector>
#include<array>

#include "fishStrategy.hpp"
#include "constants.hpp"

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
    sf::Vector2f getVelocity()
    {
        return velocity;
    }
};


class SensoryState
{
public:
    std::array<float, 8> lidar;
};


class CircularEater : public HasCenter
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
};

class Fish : public HasCenter, public HasVelocity
{
private:
    bool dead;
public:
    SensoryState sensory;
    Fish(sf::Vector2f);
    void step();
    void render(sf::RenderWindow &);
    void die();
    void updateVelocity();
};