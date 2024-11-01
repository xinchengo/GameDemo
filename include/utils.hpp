#pragma once

#include<SFML/Graphics.hpp>
#include<cmath>
#include<random>

/// @brief The random number generator for the game
extern std::mt19937 gen;

/// @brief Generate a random integer in [l, r] in uniform distribution
/// @tparam T type of the random integer to be generated
/// @param l the left boundary of the interval
/// @param r the right boundary of the interval
/// @return The generated random integer
template<typename T>
T randint(T l, T r)
{
    return std::uniform_int_distribution<T>(l, r)(gen);
}

/// @brief Generate a random real number in [l, r] in uniform distribution
/// @tparam T type of the random real number to be generated
/// @param l the left boundary of the interval
/// @param r the right boundary of the interval
/// @return The generated random real number
template<typename T>
T randBetween(T l, T r)
{
    return std::uniform_real_distribution<T>(l, r)(gen);
}

/// @brief Generate a random color (0xRRGGBBAA)
/// @return The generated random color
/// @note Result may be not opaque, to generate an opaque color, 
///       please use `randOpaqueColor()` instead.
sf::Int32 randColor();

/// @brief Generate a random opaque color (0xRRGGBBff)
/// @return The generated random color
sf::Int32 randOpaqueColor();

/// @brief Calculate the euclidean distance between two points `a` and `b`
/// @return The value of a.x * b.y - a.y * b.x
float dis2(sf::Vector2f, sf::Vector2f);

/// @brief Calculate the cross product between two vectors `a` and `b`
/// @return The value of a.x * b.y - a.y * b.x
float cross(sf::Vector2f, sf::Vector2f);

/// @brief Calculate the dot product between two vectors `a` and `b`
/// @return The value of a.x * b.x + a.y * b.y
float dot(sf::Vector2f, sf::Vector2f);

/// @brief
/// Calculate the maximum length the vector `a`, starting from (0, 0),
/// can extend before hitting the circle centering at `P` with radius `r`.
///
/// @param  a the vector
/// @param  P center of the circle 
/// @param  r radius of the circle
///
/// @return A float, indicating the maximum distance.
/// If the vector can stretch indefinitely, returns `INFINITY`
/// 
/// @note The function assumes that point (0, 0) is not within 
float disVecCirc(sf::Vector2f, sf::Vector2f, float);
sf::Vector2f rotate(sf::Vector2f, float);

/// @brief Softmax function for x
template<typename T>
T softmax(T x)
{
    return 1 / (1 + std::exp(-x));
}