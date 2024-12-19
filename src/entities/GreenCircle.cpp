#include "GreenCircle.hpp"

#include "SFML/Graphics.hpp"
#include "utilities/config.hpp"
#include "utilities/mathUtils.hpp"
#include "utilities/AssetManager.hpp"

GreenCircle::GreenCircle(sf::Vector2f position)
{
    center = position;
    float speed = randBetween(config.greenCircleSpeedMin, config.greenCircleSpeedMax);
    velocity = randVecWithLength(speed);
}

void GreenCircle::step(float time)
{
    center += velocity * time;
}

void GreenCircle::render(sf::RenderWindow &window)
{   
    sf::Sprite shape;
    shape.setTexture(assetManager.texture.get("greenCircle"), true);
    shape.setOrigin(shape.getGlobalBounds().getSize() * 0.5f);
    shape.setPosition(center);

    window.draw(shape);
}