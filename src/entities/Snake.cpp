#include "Snake.hpp"

#include "SFML/Graphics.hpp"
#include "utilities/config.hpp"
#include "utilities/mathUtils.hpp"
#include "utilities/typeUtils.hpp"
#include "utilities/AssetManager.hpp"

void Snake::extractEnclosedParts()
{
    using namespace Clipper2Lib;
    
    PathsD snake = {toPath<double>(body)};
    // If the length of the snake follows a certain criteria, include the segment connecting
    // the two ends of the snake.
    if(segmentConnectingEnds() == true)
    {
        snake.emplace_back();
        snake.back().emplace_back(toPoint<double>(body.front()));
        snake.back().emplace_back(toPoint<double>(body.back()));
    }
    snake = InflatePaths(snake, CONST::SNAKE_CIRCLE_SIZE, JoinType::Round, EndType::Round);
    // Now within it stores the polygons representing the region covered by the snake
    
    ClipperD clipper;
    clipper.AddSubject(snake);
    clipper.PreserveCollinear(false);
    // extract the enclosed (NonZero) parts of the region ENCIRCLEED by the snake
    clipper.Execute(ClipType::Union, FillRule::NonZero, predatorPolygons);

    // Shrink the polygon by 0.25 pixels to avoid potential problems in `drawPolygonIndicator`
    predatorPolygons = InflatePaths(predatorPolygons, -0.25, JoinType::Round, EndType::Polygon);
    predatorPolygons = SimplifyPaths(predatorPolygons, 0.5);

    // Remove all degenerate polygons (vertices < 3)
    predatorPolygons.erase(
    std::remove_if(predatorPolygons.begin(), predatorPolygons.end(), 
                   [](const auto& polygon) { return polygon.size() < 3; }),
    predatorPolygons.end());
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
    auto triags = poly.GetTriangles();

    for(auto &triangle : triags)
    {
        sf::VertexArray tri(sf::Triangles, 3);
        tri[0].position = sf::Vector2f(toVec<float>(*triangle->GetPoint(0)));
        tri[1].position = sf::Vector2f(toVec<float>(*triangle->GetPoint(1)));
        tri[2].position = sf::Vector2f(toVec<float>(*triangle->GetPoint(2)));
        tri[0].color = sf::Color(0x5b9bd5ff);
        tri[1].color = sf::Color(0x5b9bd5ff);
        tri[2].color = sf::Color(0x5b9bd5ff);
        
        window.draw(tri);
    }

    for(auto &i : seq)
        delete i;
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
    return body.size() * CONST::SNAKE_SEGMENT_SPACING;
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
    for (int i = 0; i < length; i++)
    {
        body.emplace_back(0.f, 0.f + CONST::SNAKE_SEGMENT_SPACING * i);
    }
}
Snake::Snake(sf::Vector2f position, int length) : Snake(length)
{
    for(auto &point : body)
    {
        point += position;
    }
}
sf::Vector2f Snake::headPos()
{
    return body.front();
}
std::vector<sf::Vector2f> &Snake::getPredatorList()
{
    return body;
}
void Snake::step()
{
    sf::Vector2f currentVelocity = velocity;
    // Move the head
    body.front() += currentVelocity;
    
    // Move the rest of the segments
    for(size_t i = 1; i < body.size(); i++)
    {
        currentVelocity = - currentVelocity
            + 2.0f * projectedOnto(currentVelocity, body[i - 1] - body[i]);
        body[i] += currentVelocity;
    }
    // Note that because floating point errors accumulate, the actual distance
    // between the segments may not be exactly `CONST::SNAKE_SEGMENT_SPACING`,
    // but in a single round, the error is negligible. (never exceeding 1e-2)

    // Find the maximum difference between the actual distance
    // of body[i] and body[i-1] and the desired distance
    float maxDiff = 0.0f;
    for(size_t i = 1; i < body.size(); i++)
    {
        float diff = dis2(body[i], body[i - 1]) - CONST::SNAKE_SEGMENT_SPACING;
        maxDiff = std::max(maxDiff, std::abs(diff));
    }
    std::cout << "Max diff: " << maxDiff << std::endl;
}
void Snake::render(sf::RenderWindow& window)
{   
    extractEnclosedParts();

    for(auto &polygon : predatorPolygons)
        drawPolygonIndicator(polygon, window);

    if(segmentConnectingEnds())
    {
        sf::RectangleShape line;
        line.setFillColor(sf::Color::White);
        line.setPosition(body.front());
        line.setSize(sf::Vector2f(dis2(body.front(), body.back()), 3.0f));
        line.setRotation(std::atan2((body.back()-body.front()).y, (body.back()-body.front()).x)/CONST::PI*180);
        window.draw(line);
    }

    auto& assetManager = AssetManager::getInstance();
    
    sf::Sprite shape;
    shape.setTexture(assetManager.texture.get("snakeBody"), true);
    shape.setOrigin(shape.getGlobalBounds().getSize() * 0.5f);

    for(auto &it : body)
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
    // Handle growth of the snake
    sf::Vector2f tailDirection;
    if(body.size() > 1)
    {
        // Calculate the direction from the second last segment to the last segment
        tailDirection = body.back() - body[body.size() - 2];
    }
    else
    {
        // If the snake has only one segment, use the negative of the velocity normalized
        tailDirection = normalizeVec(-velocity, CONST::SNAKE_SEGMENT_SPACING);
    }

    // Add `count` segments to the snake
    for(int i = 0; i < count; i++)
    {
        body.push_back(body.back() + tailDirection);
    }
}
void Snake::lengthen(float length)
{
    partialGrowth += length;
    int count = int(partialGrowth + CONST::FLOAT_EPS);
    partialGrowth -= count;
    lengthen(count);
}

bool Snake::hasEaten(sf::Vector2f point)
{
    using namespace Clipper2Lib;
    for(auto &polygon : predatorPolygons)
    {
        auto result = PointInPolygon(toPoint<double>(point), polygon);
        if(result != PointInPolygonResult::IsOutside)
        {
            return true;   
        }
    }
    return false;
}
