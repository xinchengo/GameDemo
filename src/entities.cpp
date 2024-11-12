#include<SFML/Graphics.hpp>
#include<cfloat>
#include<algorithm>
#include<array>

#include "entities.hpp"
#include "utils.hpp"
#include "constants.hpp"

CircularEater::CircularEater()
{
    setRandomVelocity(randBetween(0.8f, 1.2f));

    center = sf::Vector2f(0.0f, 0.0f);
    radius = 0.0f;
    growthRate = 0.1f;
    color = sf::Color(randOpaqueColor());
}
CircularEater::CircularEater(sf::Vector2f position)
{
    setRandomVelocity(randBetween(0.8f, 1.2f));

    center = position;
    radius = 0.0f;
    growthRate = 0.1f;
    color = sf::Color(randOpaqueColor());
}
CircularEater::CircularEater(float growth_rate)
{
    setRandomVelocity(randBetween(0.8f, 1.2f));

    center = sf::Vector2f(0.0f, 0.0f);
    radius = 0.0f;
    growthRate = growth_rate;
    color = sf::Color(randOpaqueColor());
}
void CircularEater::step()
{
    radius += growthRate;
    center += velocity;
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
void CircularEater::bounce(uint8_t ind)
{
    if(ind & 1)
        velocity.x = -velocity.x;
    if(ind & 2)
        velocity.y = -velocity.y;
}
Fish::Fish(sf::Vector2f coord, std::unique_ptr<FishStrategy> stra) : strategy(std::move(stra))
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

void Snake::extract_segments()
{
    seg.clear();

    size_t ind = 0;
    for (; ind < body.size(); ind += tightness)
        seg.push_back(body[body.size() - ind - 1]);
}
Snake::Snake(int length)
{
    velocity = sf::Vector2f(0.f, -CONST::SNAKE_SPEED);
    for (int i = length - 1; i >= 0; i--)
    {
        body.emplace_back(0.f, 0.f + CONST::SNAKE_SPEED * i);
    }
    tightness = 10;
    queued_length = 0;
}
sf::Vector2f Snake::headPos()
{
    return body.back();
}
void Snake::step()
{
    body.push_back(body.back() + velocity);
    if (queued_length == 0)
        body.erase(body.begin());
    else
        queued_length--;
}
void Snake::render(sf::RenderWindow& window)
{
    extract_segments();

    sf::CircleShape shape(CONST::SNAKE_CIRCLE_SIZE);
    shape.setOrigin(CONST::SNAKE_CIRCLE_SIZE / 2, CONST::SNAKE_CIRCLE_SIZE / 2);
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    for(auto &it : seg)
    {
        shape.setPosition(it);
        window.draw(shape);
    }
}
void Snake::setVelocityFromMousePos(sf::RenderWindow& window)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    sf::Vector2f id = worldPos - headPos(), rd = velocity;

    float d = angle_difference(id, velocity);
    if (d > 0.05f)
    {
        velocity = rotate(velocity, 0.05f);
    }
    else if (d < 0.05f)
    {
        velocity = rotate(velocity, -0.05f);
    }
    else
    {
        velocity = id * CONST::SNAKE_SPEED / std::hypot(id.x, id.y);
    }
}
void Snake::lengthen(int count)
{
    queued_length += count;
}