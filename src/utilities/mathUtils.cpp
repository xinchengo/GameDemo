#include<cfloat>
#include<cmath>

#include "mathUtils.hpp"
#include "config.hpp"

std::mt19937 gen;

sf::Int32 randColor()
{
    return gen();
}
sf::Int32 randOpaqueColor()
{
    return gen() | 0xff;
}

float normalizeAngle(float x)
{
    float a = std::fmod(x + CONST::PI, 2.0f * CONST::PI);
    if(a <= 0.0f)
        a += 2.0f * CONST::PI;
    return a - CONST::PI;
}

float dis2(sf::Vector2f a, sf::Vector2f b)
{
    return std::hypot(a.x - b.x, a.y - b.y);
}

float cross(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.y - a.y * b.x;
}
float dot(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}
float length(sf::Vector2f a)
{
    return std::hypot(a.x, a.y);
}
float angleDifference(sf::Vector2f a, sf::Vector2f b)
{
    return -std::atan2(a.x * b.y - a.y * b.x, a.x * b.x + a.y * b.y);
}

float disVecCirc(sf::Vector2f a, sf::Vector2f P, float r)
{
    if(dot(a, P) <= 0.0f || std::abs(cross(a, P) / length(a)) >= r)
        return INFINITY;
    else
        return (dot(a, P) / length(a)) - sqrt(r * r - std::pow(cross(a, P) / length(a), 2));
}
sf::Vector2f rotate(sf::Vector2f a, float x)
{
    return sf::Vector2f(a.x * cos(x) - a.y * sin(x),
        a.x * sin(x) + a.y * cos(x));
}

sf::Vector2f randVecWithLength(float length)
{
    float arg = randBetween(-CONST::PI, CONST::PI);
    return rotate(sf::Vector2f(length, 0), arg);
}
sf::Vector2f randPointInScreen(float width, float height)
{
    float x = randBetween(0.0f, width);
    float y = randBetween(0.0f, height);
    return sf::Vector2f(x, y);
}

float mutate(float x)
{
    if(randBetween(0.0f, 1.0f) <= 0.001)
    {
        x *= randBetween(-2.0f, 2.0f);
    }
    if(randBetween(0.0f, 1.0f) <= 0.005)
    {
        x *= randBetween(0.8f, 1.2f);
    }
    if(randBetween(0.0f, 1.0f) <= 0.005)
    {
        x += randBetween(-0.01f, 0.01f);
    }
    return x;
}