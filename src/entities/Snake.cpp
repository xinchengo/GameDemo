#include "Snake.hpp"
#include "utilities/config.hpp"
#include "SFML/Graphics.hpp"
#include "utilities/config.hpp"
#include "utilities/mathUtils.hpp"
#include "utilities/typeUtils.hpp"
#include "utilities/AssetManager.hpp"
#define  xxx (body.str[i+1].SnakePosition-body.str[i].SnakePosition)
void Snake::extractSegments()
{
    seg.clear();

    size_t ind = 0;
    for (; ind < body.str.size(); ind += tightness)
        seg.push_back(body.str[body.str.size()- ind - 1].SnakePosition);
}

void Snake::extractEnclosedParts()
{
    using namespace Clipper2Lib;
    
    PathsD snake = {toPath<double>(seg)};
    // If the length of the snake follows a certain criteria, include the segment connecting
    // the two ends of the snake.
    if(segmentConnectingEnds() == true)
    {
        snake.emplace_back();
        snake.back().emplace_back(toPoint<double>(seg.front()));
        snake.back().emplace_back(toPoint<double>(seg.back()));
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
}

std::vector<sf::Vector2f> &Snake::getPredatorList()
{
    return seg;
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
    else if(dis2(body.str.front().SnakePosition, body.str.back().SnakePosition)
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
        body.str.push_back({{0.f, 0.f + CONST::SNAKE_SPEED * i},velocity});
        
    }
    tightness = 10;
    queued_length = 0;
    EatenFishNumber=0;
    NewEat=0;
}
Snake::Snake(sf::Vector2f position, int length) : Snake(length)
{
    for(auto &point : body.str)
    {
        point.SnakePosition+= position;
    }
}
sf::Vector2f Snake::headPos()
{
    return  body.str.back().SnakePosition ;
}
void Snake::step()
{
    
    
    if(NewEat>=2){
      NewEat-=2;body.str.push_back({body.str.back().SnakePosition+velocity,velocity});
        
        }
        else {body.str.back().SnakePosition+=velocity;
    for(int i=body.str.size()-2;i>=0;i--)
    {
      //if(NewEat)body.str[i].SnakePosition  =body.str[i+1].SnakePosition - xxx/(  std::hypot(xxx.x,xxx.y)  )*(float)(1+EatenFishNumber*CONST::GROWTH_PERCENTAGE)*(float)(std::hypot(velocity.x, velocity.y));
 
 
 
 
 body.str[i].SnakePosition+=body.str[i].SnakeSpeed;
body.str[i].SnakeSpeed=normalizeVec(sf::Vector2f (body.str[i+1].SnakePosition-body.str[i].SnakePosition) ,std::hypot(velocity.x, velocity.y) );
       
        }

}
}
void Snake::render(sf::RenderWindow& window)
{
    extractSegments();
    
    extractEnclosedParts();

    for(auto &polygon : predatorPolygons)
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
 shape.setScale((1+EatenFishNumber * CONST::GROWTH_PERCENTAGE) ,(1+EatenFishNumber * CONST::GROWTH_PERCENTAGE) );
 //body.unit_dis =  1+NewEat * CONST::GROWTH_PERCENTAGE;
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

bool Snake::hasEaten(sf::Vector2f point)
{
    using namespace Clipper2Lib;
    for(auto &polygon : predatorPolygons)
    {
        auto result = PointInPolygon(toPoint<double>(point), polygon);
        if(result != PointInPolygonResult::IsOutside)
        {   EatenFishNumber++;
        NewEat+=10;
               
            return true;   
        }
    }
    return false;
}

