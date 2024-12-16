#include <memory>

#include "GameRunner.hpp"
#include "utilities/mathUtils.hpp"
#include "utilities/config.hpp"

GreenCircle* GameRunner::createGreenCircle(sf::Vector2f pos)
{
    return new GreenCircle(pos);
}
uint8_t GameRunner::exceedBoundary(sf::Vector2f pos)
{
    return (pos.x < 0.0f || pos.x > width) | ((pos.y < 0.0f || pos.y > height) << 1);
}

GameRunner::GameRunner(float width, float height) : 
    width(width), height(height), snake(), frameNumber(0), fish(width, height) { }

void GameRunner::newFish(int cnt)
{
    fish.createBoids(cnt);
}

void GameRunner::newGreenCircles(int cnt)
{
    for(int i=0; i<cnt; i++)
    {
        greenCircles.emplace_back(createGreenCircle(randPointInScreen(width, height)));
    }
}
void GameRunner::newSnake(sf::Vector2f position, int length)
{
    snake = std::make_unique<Snake>(position, length);
}
void GameRunner::step()
{
    frameNumber++;
    // the snake moves
    if(snake) // if the snake exists
    {
        snake->step();
        // set the snake as a list of predators
        fish.setPredators(snake->getPredatorList());
    }
    // the fish moves
    fish.step();
    // The greenCircle moves
    for(auto &circ : greenCircles)
    {
        circ->step();
        // the green circle bounces on hitting the boundary
        circ->bounce(exceedBoundary(circ->getCenter()));
    }
    if(snake)
    {
        // Remove all the green circles been eaten
        for(size_t i = 0; i < greenCircles.size(); i++)
        {
            if(snake->hasEaten(greenCircles[i]->getCenter()))
            {
                // Remove the green circle from list
                std::swap(greenCircles[i], greenCircles.back());
                eatenGreenCircles.emplace_back(std::move(greenCircles.back()));
                greenCircles.pop_back();
            }
        }
        // Remove all the fish been eaten
        auto hasEaten = [&](sf::Vector2f point) {return snake->hasEaten(point); };
        fish.removeBoidsEaten(hasEaten);
    }
}