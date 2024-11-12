#pragma once

#include<vector>
#include<array>
#include<memory>

#include "constants.hpp"
#include "utils.hpp"
#include "properties.hpp"
#include "strategies.hpp"


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

class GreenCircle : public HasCenter, public HasVelocity
{

};

class Snake : public HasVelocity
{
private:
    /*
    * `body`: Each snake is stored as a sequence of points.
    *   On every update, a new element body.back() + velocity
    *   is added, and the first element will be removed if there is
    *   no element on queue
    * `queued_length`: number of elements on queue
    */
    std::vector<sf::Vector2f> body, seg;
    int queued_length;
    int tightness;

    void extract_segments();

public:
    Snake(int length);
    sf::Vector2f headPos();
    void step();
    void render(sf::RenderWindow& window);
    void setVelocityFromMousePos(sf::RenderWindow& window);
    void lengthen(int count);
};