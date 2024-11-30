#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "utilities/properties.hpp"
#include "./strategies/strategies.hpp"

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