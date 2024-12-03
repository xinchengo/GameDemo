#pragma once

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "mathUtils.hpp"

/// @brief Property of an object for having a center
class HasCenter
{
protected:
    sf::Vector2f center;
public:
    /// @brief Set `C` as the center of the object
    /// @param C the center
    void setCenter(sf::Vector2f& C)
    {
        center = C;
    }
    /// @brief Get the center of the object
    /// @return its center
    sf::Vector2f getCenter()
    {
        return center;
    }
};

/// @brief Property of an object for having a velocity
class HasVelocity
{
protected:
    sf::Vector2f velocity;
public:
    /// @brief Set the velocity of the object to `C`
    /// @param C the velocity
    void setVelocity(sf::Vector2f& C)
    {
        velocity = C;
    }
    /// @brief Set the speed of the object to `x`, but with a random direction
    /// @param x the intended speed 
    void setRandomVelocity(float x)
    {
        float arg = randBetween(-CONST::PI, CONST::PI);
        velocity = rotate(sf::Vector2f(x, 0.0f), arg);
    }
    /// @brief Get the velocity of the object
    /// @return its velocity
    sf::Vector2f getVelocity()
    {
        return velocity;
    }
    /// @brief Make the object bounce when it hits the boundary
    /// @param ind this parameter determines how the object
    /// bounces: with a lowest bit of 1, the object bounces
    /// horizontally; and if the second lowest bit is 1,
    /// the object bounces vertically.
    /// @note `ind` should be obtained from `GameRunner::exceedBoundary()`
    void bounce(uint8_t ind)
    {
        if(ind & 1)
            velocity.x = -velocity.x;
        if(ind & 2)
            velocity.y = -velocity.y;
    }
};