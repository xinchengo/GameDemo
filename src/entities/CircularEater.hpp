#pragma once

#include <SFML/Graphics.hpp>

#include "utilities/properties.hpp"

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
};