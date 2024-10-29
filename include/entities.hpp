#pragma once

#include<array>
#include<vector>

class SensoryState
{
public:
    std::array<float, 8> lidar;
};

class CircularEater
{
private:
    sf::Vector2f center;
    float currentRadius;
    float growthRate;
    sf::Color color;
public:
    CircularEater();
    CircularEater(sf::Vector2f);
    CircularEater(float);
    void step();
    void render(sf::RenderWindow &);
    sf::Vector2f getCenter();
    float getRadius();
};

class Fish
{
private:
    sf::Vector2f center;
    sf::Vector2f velocity;
    bool dead;
public:
    SensoryState sensory;
    Fish(sf::Vector2f);
    void step();
    void render(sf::RenderWindow &);
    sf::Vector2f getCenter();
    sf::Vector2f getVelocity();
    void die();
    void updateVelocity();
};