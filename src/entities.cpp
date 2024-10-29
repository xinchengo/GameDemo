#include<SFML/Graphics.hpp>

#include "entities.hpp"
#include "utils.hpp"

CircularEater::CircularEater()
{
    center = sf::Vector2f(0.0f, 0.0f);
    currentRadius = 0.0f;
    growthRate = 0.1f;
    color = sf::Color(randOpaqueColor());
}
CircularEater::CircularEater(sf::Vector2f position)
{
    center = position;
    currentRadius = 0.0f;
    growthRate = 0.1f;
    color = sf::Color(randOpaqueColor());
}
CircularEater::CircularEater(float growth_rate)
{
    center = sf::Vector2f(0.0f, 0.0f);
    currentRadius = 0.0f;
    growthRate = growth_rate;
    color = sf::Color(randOpaqueColor());
}
void CircularEater::step()
{
    currentRadius += growthRate;
}
void CircularEater::render(sf::RenderWindow &window)
{
    if(currentRadius == 0.0f)
        return;
    sf::CircleShape circ(currentRadius);
    circ.setOrigin(currentRadius, currentRadius);
    circ.setFillColor(color);
    circ.setPosition(center);

    window.draw(circ);
}
sf::Vector2f CircularEater::getCenter()
{
    return center;
}
float CircularEater::getRadius()
{
    return currentRadius;
}

Fish::Fish(sf::Vector2f coord)
{
    center = coord;
    velocity = sf::Vector2f(0.0f, 0.0f);
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
sf::Vector2f Fish::getCenter()
{
    return center;
}
void Fish::die()
{
    dead = true;
}