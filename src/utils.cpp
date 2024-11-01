#include<cfloat>
#include<cmath>

#include "utils.hpp"
#include "constants.hpp"

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

float disVecCirc(sf::Vector2f a, sf::Vector2f P, float r)
{
    /*
    Compute the longest length the vector `a`
    can reach before hitting the circle centering at `P` with
    radius `r`.

    Assuming that r < |OP|.
    */
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