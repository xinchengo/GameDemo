#include "GreenCircle.hpp"

#include "SFML/Graphics.hpp"
#include "utilities/config.hpp"
#include "utilities/utils.hpp"
#include "utilities/AssetManager.hpp"

GreenCircle::GreenCircle(sf::Vector2f position)
{
    center = position;
    float speed = randBetween(CONST::GREEN_CIRCLE_SPEED_MIN, CONST::GREEN_CIRCLE_SPEED_MAX);
    velocity = randVecWithLength(speed);
}

void GreenCircle::step()
{
    center += velocity;
}

void GreenCircle::render(sf::RenderWindow &window)
{
    auto& assetManager = AssetManager::getInstance();
    
    sf::Sprite shape;
    shape.setTexture(assetManager.texture.get("greenCircle"), true);
    shape.setOrigin(shape.getGlobalBounds().getSize() * 0.5f);
    shape.setPosition(center);

    window.draw(shape);
}