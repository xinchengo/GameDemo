#pragma once

#include<SFML/Graphics.hpp>
#include<cmath>
#include<random>

extern std::mt19937 gen;

template<typename T>
T randint(T l, T r)
{
    return std::uniform_int_distribution<T>(l, r)(gen);
}
template<typename T>
T randBetween(T l, T r)
{
    return std::uniform_real_distribution<T>(l, r)(gen);
}

sf::Int32 randColor();
sf::Int32 randOpaqueColor();

float dis2(sf::Vector2f, sf::Vector2f);
float cross(sf::Vector2f, sf::Vector2f);
float dot(sf::Vector2f, sf::Vector2f);
float disVecCirc(sf::Vector2f, sf::Vector2f, float);
sf::Vector2f rotate(sf::Vector2f, float);