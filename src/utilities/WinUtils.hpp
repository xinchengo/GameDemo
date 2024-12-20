#pragma once

#include <SFML/Graphics.hpp>

template<typename T>
sf::View getView(sf::Vector2<T> windowSize, float scale)
{
    sf::FloatRect rect(0, 0, static_cast<float>(windowSize.x) / scale, 
        static_cast<float>(windowSize.y) / scale);
    return sf::View(rect);
}

float getWindowScale();
void enableResize(sf::RenderWindow &window);
void disableResize(sf::RenderWindow &window);
void toggleFullscreen(sf::RenderWindow &window);
