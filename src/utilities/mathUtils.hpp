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

/// @brief normalize the angle x to [-PI, PI]
/// @param x angle to be normalized
/// @return 
float normalizeAngle(float);

/// @brief Calculate the euclidean distance between two points `a` and `b`
/// @return The value of a.x * b.y - a.y * b.x
float dis2(sf::Vector2f, sf::Vector2f);

/// @brief Calculate the cross product between two vectors `a` and `b`
/// @return The value of a.x * b.y - a.y * b.x
float cross(sf::Vector2f, sf::Vector2f);

/// @brief Calculate the dot product between two vectors `a` and `b`
/// @return The value of a.x * b.x + a.y * b.y
float dot(sf::Vector2f, sf::Vector2f);

/// @brief Calculate the projection of vector `a` onto vector `b`
/// @return The vector projected onto `b`
sf::Vector2f projectedOnto(sf::Vector2f, sf::Vector2f);

/// @brief Normalize the length of vector `a` to `desiredLength`
/// @return The vector normalized to desiredLength
sf::Vector2f normalizeVec(sf::Vector2f a, float desiredLength);

/// @brief Clamp the length of vector `a` to `lengthLimit`
/// @return The vector clamped to `lengthLimit`
sf::Vector2f clampVec(sf::Vector2f a, float lengthLimit);

/// @brief Calculate arg(a) - arg(b) normalized to (-pi/2, pi/2)
/// @return the value of norm(arg(a) - arg(b))
/// @note cross product computes cos(arg(b) - arg(a)), so a minus sign is added
float angleDifference(sf::Vector2f, sf::Vector2f);

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

/**
 * @brief Utility function to calculate the orientation of the triplet (p, q, r)
 * 
 * @return 0 (p, q, r) are collinear
 * @return 1 (p, q, r) are in clockwise order
 * @return 2 (p, q, r) are in counter-clockwise order
 */
int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);
/**
 * @brief Helper function for `intersect()` to check if point q lies on segment pr
 * 
 * @return whether point q lies on segment pr
 * @warning DO NOT CALL THIS FUNCTION!
 */
bool helper_onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);

/**
 * @brief Calculate whether the segments ab and cd intersects.
 * 
 * @return Whether the segments ab and cd intersects.
 */
bool doIntersect(sf::Vector2f p1, sf::Vector2f q1, sf::Vector2f p2, sf::Vector2f q2);
sf::Vector2f rotate(sf::Vector2f, float);
/// @brief Softmax function for x
template<typename T>
T softmax(T x)
{
    return 1 / (1 + std::exp(-x));
}

/// @brief Generate a vector with a length of `length`
/// but in random direction
/// @param length the desired length of the vector
/// @return the generated vector
sf::Vector2f randVecWithLength(float);

/// @brief Generate a random point in screen with dimension `width` times `height`
/// @param width the width of the screen
/// @param height the height of the screen
/// @return the generated point
sf::Vector2f randPointInScreen(float, float);

/// @brief Mutate a float `x`, implemetation is user defined
/// @param x the float to be mutated
/// @return the float after mutation
float mutate(float);