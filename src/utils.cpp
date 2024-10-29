#include "utils.hpp"

float dis2(sf::Vector2f a, sf::Vector2f b)
{
    return std::hypot(a.x - b.x, a.y - b.y);
}
