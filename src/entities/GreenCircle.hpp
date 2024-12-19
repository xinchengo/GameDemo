#pragma once

#include <SFML/Graphics.hpp>

#include "utilities/properties.hpp"

class GreenCircle : public HasCenter, public HasVelocity
{
private:
public:
    GreenCircle(sf::Vector2f position);
    void step(float time);
    void render(sf::RenderWindow& window);
};