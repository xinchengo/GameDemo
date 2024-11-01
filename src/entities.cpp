#include<SFML/Graphics.hpp>
#include<cfloat>
#include<algorithm>
#include<array>

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
    velocity = strategy.predictVelocity(sensory, velocity);
    // velocity += sf::Vector2f(randBetween(-0.01f, 0.01f), randBetween(-0.01f, 0.01f));
}

FishStrategy::FishStrategy()
{
    for(auto &x : a)
        x = randBetween(-0.1f / CONST::LIDAR_CNT, 0.1f / CONST::LIDAR_CNT);
    for(auto &x : b)
        x = randBetween(-0.1f / CONST::LIDAR_CNT, 0.1f / CONST::LIDAR_CNT);
    c = randBetween(-0.1f, 0.1f);
    acceleration_bias = randBetween(-0.1f, 0.1f);
}
sf::Vector2f FishStrategy::predictVelocity(SensoryState &sense, sf::Vector2f velocity)
{
    auto speed = std::hypot(velocity.x, velocity.y);
    
    float prod1 = 0, prod2 = 0;
    for(size_t i=0; i<CONST::LIDAR_CNT; i++)
        prod1 += a[i] * sense.lidar[i], prod2 += b[i] * sense.lidar[i];
    prod1 += speed * c;
    prod1 += acceleration_bias;

    float d_v = CONST::FISH_SPEED_CHANGE_MAX * std::tanh(prod1);
    float d_o = CONST::FISH_DIRRECTION_CHANGE_MAX * std::tanh(prod2);
    
    return rotate(velocity * (1+d_v), d_o);
}