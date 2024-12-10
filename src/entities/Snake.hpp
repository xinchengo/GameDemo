#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <clipper2/clipper.h>
#include <poly2tri/poly2tri.h>

#include "utilities/properties.hpp"

class Snake : public HasVelocity
{
private:
    /// @brief Each snake is stored as a sequence of points.
    /// On every update, a new element `body.back() + velocity`
    /// is added, and the first element will be removed if there is
    /// no element in queue
    std::vector<sf::Vector2f> body;
    std::vector<sf::Vector2f> seg;
    Clipper2Lib::PathsD polygons;
    /// @brief number of elements on queue
    int queued_length;
    int tightness;

    void extract_segments();
    void extract_enclosed_parts();
    void draw_polygon_indicator(Clipper2Lib::PathD &polygon, sf::RenderWindow &window);

     /**
     * @brief Helper function to determine whether the snake is in eating
     * state or not.
     * 
     * @return Whether the segment between `seg.front()` and `seg.back()`
     * intersects with other body parts
     */
    bool intersect();
    /**
     * @brief Return the length of the snake (`body.size() * tightness`)
     * 
     * @return the length of the snake
     */
    float snakeLength();
    bool isInPredatorMode();

public:
    Snake(int length);
    sf::Vector2f headPos();
    void step();
    void render(sf::RenderWindow& window);
    void setVelocityFromMousePos(sf::RenderWindow& window);
    void lengthen(int count);
};