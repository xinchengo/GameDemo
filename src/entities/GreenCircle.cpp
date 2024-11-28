#include "GreenCircle.hpp"

#include "SFML/Graphics.hpp"
#include "utilities/config.hpp"
#include "utilities/utils.hpp"

GreenCircle::GreenCircle(sf::Vector2f position)
{
    center = position;
    float speed = randBetween(CONST::GREEN_CIRCLE_SPEED_MIN, CONST::GREEN_CIRCLE_SPEED_MAX);
    velocity = randVecWithLength(speed);
}

void GreenCircle::step()
{
    center += velocity;
}

void GreenCircle::render(sf::RenderWindow &window)
{
    sf::CircleShape circ(CONST::GREEN_CIRCLE_RADIUS);
    circ.setOrigin(CONST::GREEN_CIRCLE_RADIUS, CONST::GREEN_CIRCLE_RADIUS);
    circ.setPosition(center);
    circ.setOutlineColor(sf::Color::Green);
    circ.setFillColor(sf::Color::Transparent);
    circ.setOutlineThickness(CONST::GREEN_CIRCLE_OUTLINE_THICKNESS);
    window.draw(circ);
}