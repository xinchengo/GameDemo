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
    float speed = randBetween(CONST::FISH_SPEED_MIN, CONST::FISH_SPEED_MAX);
    float arg = randBetween(-CONST::PI, CONST::PI);
    velocity = rotate(sf::Vector2f(speed, 0), arg);
    deathTime = CONST::FRAME_CNT_INFINITY;
}
void Fish::step()
{
    center += velocity;
}
void Fish::render(sf::RenderWindow &window)
{   
    sf::CircleShape triangle(10.0f);
    triangle.setPointCount(3);
    triangle.setOrigin(10.0f, 10.0f);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition(center);

    window.draw(triangle);
}
bool Fish::isDead(size_t frameNumber)
{
    return deathTime <= frameNumber;
}
void Fish::die(size_t frameNumber)
{
    deathTime = frameNumber;
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

    float nv = CONST::FISH_SPEED_CHANGE_MAX * std::tanh(prod1);
    float no = CONST::FISH_DIRRECTION_CHANGE_MAX * std::tanh(prod2);
    
    nv = std::clamp(speed + nv, CONST::FISH_SPEED_MIN, CONST::FISH_SPEED_MAX);
    
    return rotate((nv / speed) * velocity, no);
}