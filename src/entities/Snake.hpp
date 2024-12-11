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
    /// @brief Helper array to store the segments to be displayed
    std::vector<sf::Vector2f> seg;
    /// @brief Helper array for storing polygon areas capable of eating fish and green circles
    Clipper2Lib::PathsD predatorPolygons;
    /// @brief number of elements waiting to be added to the snake
    int queued_length;
    /// @brief 1 out of `tightness` segments will be displayed
    int tightness;

    void extractSegments();

    /**
     * @brief Helper function to extract all the regions that are in eating state.
     * 
     * After each call, `predatorPolygons` is modified to store all the regions that are in
     * eating state. `predatorPolygons` can be empty when no region is in predator mode.
     * 
     * @note `extractSegments()` must be called prior to the call of this function.
     * 
     * The function stores its result in `predatorPolygons`, all its contents will be overwritten.
     */
    void extractEnclosedParts();

    /**
     * @brief Helper function to draw the polygon indicator for predator mode
     * onto screen
     * 
     * @param polygon a single polygon
     * @param window the window for the polygon to be drawn
     * 
     * @note the `poly2tri` library is not very robust. Make sure that the polygon
     * is simple and does not contain overlapping points or colinear segments.
     * 
     * The function gets its input from `predatorPolygons`. Typically used after a call of
     * `extractEnclosedParts()`.
     */
    void drawPolygonIndicator(Clipper2Lib::PathD &polygon, sf::RenderWindow &window);

     /**
     * @brief Helper function to determine whether the snake is in eating
     * state or not.
     * 
     * @return Whether the segment between `seg.front()` and `seg.back()`
     * intersects with other body parts
     */
    bool intersect();
    /**
     * @brief Return the length of the snake on the screen (`body.size() * tightness`)
     * 
     * @return the length of the snake (in pixels)
     */
    float snakeLength();
    /**
     * @brief Helper function to determine whether the segment connecting the head
     * and tail of the snake can enclose a region that can eat fish and green circles.
     * 
     * @return Whether the segment connecting the head and tail of the snake can
     * enclose a region in predator mode.
     */
    bool segmentConnectingEnds();

public:
    Snake(int length);
    sf::Vector2f headPos();
    void step();
    void render(sf::RenderWindow& window);
    void setVelocityFromMousePos(sf::RenderWindow& window);
    /**
     * @brief Lengthen the snake by 'count' segments
     * 
     * @param count 
     */
    void lengthen(int count);
    /**
     * @brief Determine whether a point has been eaten by the snake
     * 
     * @param point 
     * @return Whether the point has been eaten by the snake
     * 
     * @note
     */
    bool hasEaten(sf::Vector2f point);
};