#pragma once

#include <type_traits>
#include <vector>
#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include <poly2tri/poly2tri.h>

/**
 * @brief Convert a `Clipper2Lib::Point` to a `sf::Vector2`
 * 
 * @tparam TVector Number type for the coordinates of sf::Vector2
 * @tparam TPoint Number type for the coordinates of Clipper2Lib::Point
 * @param point the point to be converted
 * @return the converted vector
 */
template<typename TVector, typename TPoint>
sf::Vector2<TVector> toVec(const Clipper2Lib::Point<TPoint> &point)
{
    return sf::Vector2<TVector>(static_cast<TVector>(point.x), static_cast<TVector>(point.y));
}

/**
 * @brief Convert a `p2t::Point` to a `sf::Vector2`
 * 
 * @tparam TVector Number type for the coordinates of sf::Vector2
 * @param point the point to be converted
 * @return the converted vector
 */
template<typename TVector>
sf::Vector2<TVector> toVec(const p2t::Point &point)
{
    return sf::Vector2<TVector>(static_cast<TVector>(point.x), static_cast<TVector>(point.y));
}

/**
 * @brief Convert a `Clipper2Lib::Path` to a `std::vector<sf::Vector2>`
 * 
 * @tparam TVector Number type for the coordinates of sf::Vector2
 * @tparam TPoint Number type for the coordinates of Clipper2Lib::Point
 * @param path the path to be converted
 * @return the converted vector
 */
template<typename TVector, typename TPoint>
std::vector<sf::Vector2<TVector>> toVecOfVec(const Clipper2Lib::Path<TPoint> &path)
{
    std::vector<sf::Vector2<TVector>> vec;
    for(auto &point : path)
        vec.emplace_back(toVec(point));
}

/**
 * @brief Convert a `sf::Vector2` to a `Clipper2Lib::Point`
 * 
 * @tparam TPoint Number type for the coordinates of Clipper2Lib::Point
 * @tparam TVector Number type for the coordinates of sf::Vector2
 * @param vector the vector to be converted
 * @return the converted point
 */
template<typename TPoint, typename TVector>
Clipper2Lib::Point<TPoint> toPoint(const sf::Vector2<TVector> &vector)
{
    return Clipper2Lib::Point<TPoint>(static_cast<TPoint>(vector.x), static_cast<TPoint>(vector.y));
}

/**
 * @brief Convert a `std::vector<sf::Vector2>` to a `Clipper2Lib::Path`
 * 
 * @tparam TPoint Number type for the coordinates of Clipper2Lib::Point
 * @tparam TVector Number type for the coordinates of sf::Vector2
 * @param vector the vector to be converted
 * @return the converted path
 */
template<typename TPoint, typename TVector>
Clipper2Lib::Path<TPoint> toPath(const std::vector<sf::Vector2<TVector>> &vector)
{
    Clipper2Lib::Path<TPoint> path;
    for(auto &point : vector)
        path.emplace_back(toPoint<TPoint>(point));
    return path;
}