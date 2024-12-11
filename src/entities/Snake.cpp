#include "Snake.hpp"

#include "SFML/Graphics.hpp"
#include "utilities/config.hpp"
#include "..\utilities\mathUtils.hpp"
#include "utilities/AssetManager.hpp"

void Snake::extractSegments()
{
    seg.clear();

    size_t ind = 0;
    for (; ind < body.size(); ind += tightness)
        seg.push_back(body[body.size() - ind - 1]);
}
void Snake::extractEnclosedParts()
{
    using namespace Clipper2Lib;
    
    PathsD snake;
    snake.emplace_back();
    for(auto &i : seg)
        snake.front().emplace_back(i.x, i.y); // convert seg to PathD
    // If the length of the snake follows a certain criteria, include the segment connecting
    // the two ends of the snake.
    if(segmentConnectingEnds() == true)
    {
        snake.emplace_back();
        snake.back().emplace_back(seg.front().x, seg.front().y);
        snake.back().emplace_back(seg.back().x, seg.back().y);
    }
    snake = InflatePaths(snake, CONST::SNAKE_CIRCLE_SIZE, JoinType::Round, EndType::Round);
    // Now within it stores the polygons representing the region covered by the snake
    
    ClipperD clipper;
    clipper.AddSubject(snake);
    clipper.PreserveCollinear(false);
    // extract the enclosed (NonZero) parts of the region ENCIRCLEED by the snake
    clipper.Execute(ClipType::Union, FillRule::NonZero, polygons);

    // Shrink the polygon by 0.25 pixels to avoid potential problems in `drawPolygonIndicator`
    polygons = InflatePaths(polygons, -0.25, JoinType::Round, EndType::Polygon);
    polygons = SimplifyPaths(polygons, 0.5);
}
void Snake::drawPolygonIndicator(Clipper2Lib::PathD &polygon, sf::RenderWindow &window)
{
    std::vector<p2t::Point*> seq;
    for(auto &i : polygon)
    {
        seq.push_back(new p2t::Point((double)i.x, (double)i.y));
    }
    
    p2t::CDT poly(seq);
    poly.Triangulate();
    auto &triags = poly.GetTriangles();

    for(auto &triangle : triags)
    {
        sf::VertexArray tri(sf::Triangles, 3);
        tri[0].position = sf::Vector2f((float)triangle->GetPoint(0)->x, (float)triangle->GetPoint(0)->y);
        tri[1].position = sf::Vector2f((float)triangle->GetPoint(1)->x, (float)triangle->GetPoint(1)->y);
        tri[2].position = sf::Vector2f((float)triangle->GetPoint(2)->x, (float)triangle->GetPoint(2)->y);
        tri[0].color = sf::Color::Green;
        tri[1].color = sf::Color::Green;
        tri[2].color = sf::Color::Green;
        
        window.draw(tri);
    }

    for(auto &i : seq)
        delete i;
}
bool Snake::intersect()
{
    size_t ind = 1;
    for(; ind < seg.size() - 2; ind++)
    {
        if(doIntersect(seg[ind], seg[ind+1], seg.front(), seg.back()))
        {
            return true;
        }
    }
    return false;
}
float Snake::snakeLength()
{
    return seg.size() * tightness * CONST::SNAKE_SPEED;
}
bool Snake::segmentConnectingEnds()
{
    if(Snake::intersect())
        return false;
    else if(dis2(body.front(), body.back())
        < CONST::SNAKE_COEFFICIENT_OF_PREDATOR_MODE * snakeLength())
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
    extractSegments();
    
    extractEnclosedParts();

    for(auto &polygon : polygons)
        drawPolygonIndicator(polygon, window);

    if(segmentConnectingEnds())
    {
        sf::RectangleShape line;
        line.setFillColor(sf::Color::White);
        line.setPosition(seg.front());
        line.setSize(sf::Vector2f(dis2(seg.front(), seg.back()), 3.0f));
        line.setRotation(std::atan2((seg.back()-seg.front()).y, (seg.back()-seg.front()).x)/CONST::PI*180);
        window.draw(line);
    }

    auto& assetManager = AssetManager::getInstance();
    
    sf::Sprite shape;
    shape.setTexture(assetManager.texture.get("snakeBody"), true);
    shape.setOrigin(shape.getGlobalBounds().getSize() * 0.5f);

    for(auto &it : seg)
    {
        shape.setPosition(it);
        window.draw(shape);
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