#include "utils.hpp"

std::mt19937 gen;

sf::Int32 randColor()
{
    return gen();
}
sf::Int32 randOpaqueColor()
{
    return gen() | 0xff;
}

float dis2(sf::Vector2f a, sf::Vector2f b)
{
    return std::hypot(a.x - b.x, a.y - b.y);
}
