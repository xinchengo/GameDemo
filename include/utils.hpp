#pragma once

#include<SFML/Graphics.hpp>
#include<cmath>
#include<random>

template<typename T>
T randint(T l, T r)
{
    static std::mt19937 gen;
    return std::uniform_int_distribution<T>(l, r)(gen);
}
template<typename T>
T randBetween(T l, T r)
{
    static std::mt19937 gen;
    return std::uniform_real_distribution<T>(l, r)(gen);
}

float dis2(sf::Vector2f, sf::Vector2f);