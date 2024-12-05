#include "Snake.hpp"

#include "SFML/Graphics.hpp"
#include "utilities/config.hpp"
#include "..\utilities\mathUtils.hpp"
#include "utilities/AssetManager.hpp"

void Snake::extract_segments()
{
    seg.clear();

    size_t ind = 0;
    for (; ind < body.size(); ind += tightness)
        seg.push_back(body[body.size() - ind - 1]);
}
bool Snake::intersect()
{
    size_t ind = 1;
    for(; ind < body.size() - 2; ind++)
    {
        if(doIntersect(body[ind], body[ind+1], body.front(), body.back()))
        {
            return true;
        }
    }
    return false;
}
float Snake::snakeLength()
{
    return body.size() * tightness;
}
bool Snake::isInPredationMode()
{
    if(Snake::intersect())
        return false;
    else if(dis2(body.front(), body.back())
        < CONST::SNAKE_COEFFICIENT_OF_PREDATION_MODE * snakeLength())
        return true;
    else
        return false;
}
Snake::Snake(int length)
{
    velocity = sf::Vector2f(0.f, -CONST::SNAKE_SPEED);
    for (int i = length - 1; i >= 0; i--)
    {
        body.emplace_back(0.f, 0.f + CONST::SNAKE_SPEED * i);
    }
    tightness = 10;
    queued_length = 0;
}
sf::Vector2f Snake::headPos()
{
    return body.back();
}
void Snake::step()
{
    body.push_back(body.back() + velocity);
    if (queued_length == 0)
        body.erase(body.begin());
    else
        queued_length--;
}
void Snake::render(sf::RenderWindow& window)
{
    extract_segments();

    auto& assetManager = AssetManager::getInstance();
    
    sf::Sprite shape;
    shape.setTexture(assetManager.texture.get("snakeBody"), true);
    shape.setOrigin(shape.getGlobalBounds().getSize() * 0.5f);

    for(auto &it : seg)
    {
        shape.setPosition(it);
        window.draw(shape);
    }

    if(isInPredationMode())
    {
        sf::RectangleShape line;
        line.setFillColor(sf::Color::White);
        line.setPosition(body.front());
        line.setSize(sf::Vector2f(dis2(body.front(), body.back()), 3.0f));
        line.setRotation(std::atan2((body.back()-body.front()).y, (body.back()-body.front()).x)/CONST::PI*180);
        window.draw(line);
    }
}
void Snake::setVelocityFromMousePos(sf::RenderWindow& window)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    sf::Vector2f id = worldPos - headPos(), rd = velocity;

    float d = angleDifference(id, velocity);
    if (d > 0.05f)
    {
        velocity = rotate(velocity, 0.05f);
    }
    else if (d < 0.05f)
    {
        velocity = rotate(velocity, -0.05f);
    }
    else
    {
        velocity = id * CONST::SNAKE_SPEED / std::hypot(id.x, id.y);
    }
}
void Snake::lengthen(int count)
{
    queued_length += count;
}