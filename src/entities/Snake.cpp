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
    
    // Temporary storage for all polygons
    PolyTreeD allPolygons;
    clipper.Execute(ClipType::Union, FillRule::NonZero, allPolygons);
    
    // Extract only outer polygons (depth = 0)
    predatorPolygons.clear();
    for (const auto& child : allPolygons)
    {
        if (child->Polygon().size() >= 3)
        {  // Skip degenerate polygons
            predatorPolygons.emplace_back(child->Polygon());
        }
    }

    // Apply post-processing
    predatorPolygons = InflatePaths(predatorPolygons, -0.25, JoinType::Round, EndType::Polygon);
    predatorPolygons = SimplifyPaths(predatorPolygons, 0.5);
}
void Snake::drawPolygonIndicator(Clipper2Lib::PathD &polygon, sf::RenderWindow &window)
{
    // Convert polygon points to poly2tri points
    std::vector<p2t::Point*> poly2triPoints;
    for (const auto &point : polygon)
    {
        poly2triPoints.push_back(new p2t::Point(static_cast<double>(point.x), static_cast<double>(point.y)));
    }
    
    // Create a constrained Delaunay triangulation (CDT) from the points
    p2t::CDT cdt(poly2triPoints);
    cdt.Triangulate();
    auto triangles = cdt.GetTriangles();

    // Create a VertexArray to hold all the triangles
    sf::VertexArray triangleVertices(sf::Triangles);

    // Convert each triangle to SFML vertices and add to the VertexArray
    for (const auto &triangle : triangles)
    {
        sf::Vertex vertex0, vertex1, vertex2;
        vertex0.position = sf::Vector2f(toVec<float>(*triangle->GetPoint(0)));
        vertex1.position = sf::Vector2f(toVec<float>(*triangle->GetPoint(1)));
        vertex2.position = sf::Vector2f(toVec<float>(*triangle->GetPoint(2)));
        vertex0.color = sf::Color(CONST::SNAKE_POLYGON_INDICATOR_COLOR);
        vertex1.color = sf::Color(CONST::SNAKE_POLYGON_INDICATOR_COLOR);
        vertex2.color = sf::Color(CONST::SNAKE_POLYGON_INDICATOR_COLOR);

        triangleVertices.append(vertex0);
        triangleVertices.append(vertex1);
        triangleVertices.append(vertex2);
    }

    // Draw all the triangles at once
    window.draw(triangleVertices);

    // Clean up dynamically allocated poly2tri points
    for (auto &point : poly2triPoints)
    {
        delete point;
    }
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
void Snake::step(float time)
{
    sf::Vector2f deltaX = velocity * time;
    // Move the head
    body.front() += deltaX;
    
    // Move the rest of the segments
    for(size_t i = 1; i < body.size(); i++)
    {
        deltaX = - deltaX + 2.0f * projectedOnto(deltaX, body[i - 1] - body[i]);
        body[i] += deltaX;
    }
    // Note that because floating point errors accumulate, the actual distance
    // between the segments may not be exactly `CONST::SNAKE_SEGMENT_SPACING`,
    // but in a single round, the error is negligible. (never exceeding 1e-2)
    // float maxDiff = calculateMaxDiff();
    // std::cout << "Max diff: " << maxDiff << std::endl;
}
float Snake::calculateMaxDiff()
{
    float maxDiff = 0.0f;
    for (size_t i = 1; i < body.size(); ++i)
    {
        float diff = std::abs(dis2(body[i], body[i - 1]) - CONST::SNAKE_SEGMENT_SPACING);
        if (diff > maxDiff)
        {
            maxDiff = diff;
        }
    }
    return maxDiff;
}
void Snake::render(sf::RenderWindow& window)
{   
    extractEnclosedParts();

    for(auto &polygon : predatorPolygons)
        drawPolygonIndicator(polygon, window);

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
void Snake::setVelocityFromMousePos(sf::RenderWindow& window, float time)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    sf::Vector2f id = worldPos - headPos(), rd = velocity;

    float d = angleDifference(id, velocity);
    float maxTurnAngle = CONST::SNAKE_MAX_TURN_ANGLE * time;
    if (d > maxTurnAngle)
    {
        velocity = rotate(velocity, maxTurnAngle);
    }
    else if (d < maxTurnAngle)
    {
        velocity = rotate(velocity, -maxTurnAngle);
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
