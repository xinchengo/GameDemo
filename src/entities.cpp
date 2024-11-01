#include<SFML/Graphics.hpp>
#include<cfloat>

#include "entities.hpp"
#include "utils.hpp"
#include "constants.hpp"

CircularEater::CircularEater()
{
    center = sf::Vector2f(0.0f, 0.0f);
    radius = 0.0f;
    growthRate = 0.1f;
    color = sf::Color(randOpaqueColor());
}
CircularEater::CircularEater(sf::Vector2f position)
{
    center = position;
    radius = 0.0f;
    growthRate = 0.1f;
    color = sf::Color(randOpaqueColor());
}
CircularEater::CircularEater(float growth_rate)
{
    center = sf::Vector2f(0.0f, 0.0f);
    radius = 0.0f;
    growthRate = growth_rate;
    color = sf::Color(randOpaqueColor());
}
void CircularEater::step()
{
    radius += growthRate;
}
void CircularEater::render(sf::RenderWindow &window)
{
    if(radius == 0.0f)
        return;
    sf::CircleShape circ(radius);
    circ.setOrigin(radius, radius);
    circ.setFillColor(color);
    circ.setPosition(center);

    window.draw(circ);
}
float CircularEater::getRadius()
{
    return radius;
}

Fish::Fish(sf::Vector2f coord)
{
    center = coord;
    velocity = sf::Vector2f(randBetween(-0.1f, 0.1f), randBetween(-0.1f, 0.1f));
    dead = false;
}
void Fish::step()
{
    center += velocity;
}
void Fish::render(sf::RenderWindow &window)
{
    if(dead)
        return;
    
    sf::CircleShape triangle(10.0f);
    triangle.setPointCount(3);
    triangle.setOrigin(10.0f, 10.0f);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition(center);

    window.draw(triangle);
}
void Fish::die()
{
    dead = true;
}
void Fish::updateVelocity()
{
    velocity += sf::Vector2f(randBetween(-0.01f, 0.01f), randBetween(-0.01f, 0.01f));
}