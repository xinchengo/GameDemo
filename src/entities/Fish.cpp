#include "Fish.hpp"

#include "SFML/Graphics.hpp"
#include "utilities/config.hpp"
#include "..\utilities\mathUtils.hpp"

Fish::Fish(sf::Vector2f coord, std::unique_ptr<FishStrategy> stra) : strategy(std::move(stra))
{
    center = coord;
    float speed = randBetween(CONST::FISH_SPEED_MIN, CONST::FISH_SPEED_MAX);
    velocity = randVecWithLength(speed);
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
size_t Fish::timeOfDeath()
{
    return deathTime;
}
void Fish::die(size_t frameNumber)
{
    deathTime = frameNumber;
}
void Fish::updateVelocity()
{
    velocity = strategy->predictVelocity(sensory, velocity);
    // velocity += sf::Vector2f(randBetween(-0.01f, 0.01f), randBetween(-0.01f, 0.01f));
}