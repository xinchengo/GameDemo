#include "gameRunner.hpp"
#include "utils.hpp"

bool GameRunner::isEaten(sf::Vector2f pos)
{
    for(auto &eater : eaters)
    {
        if(dis2(eater.getCenter(), pos) < eater.getRadius())
        {
            return true;
        }
    }
    return false;
}

GameRunner::GameRunner(sf::RenderWindow &onWindow) : window(onWindow)
{
    width = window.getSize().x;
    height = window.getSize().y;

    frameNumber = 0;
    for(int i=0; i<100; i++)
    {
        fish.emplace_back(Fish(
            sf::Vector2f(randBetween(0.0f, width), randBetween(0.0f, height))));
    }
    
}
void GameRunner::step()
{
    frameNumber++;
    // Update the fish's velocities according to their strategies
    // If frameNumber satisfies a certain condition, create a new CircularEater
    if(frameNumber % 480 == 0)
    {
        eaters.emplace_back(CircularEater(
            sf::Vector2f(randBetween(0.0f, width), randBetween(0.0f, height))));
    }
    // the fish move
    for(auto &fish : fish)
    {
        fish.step();
    }
    // the eaters enlarge
    for(auto &eater : eaters)
    {
        eater.step();
    }
    // Remove all the fish been eaten
    for(auto &fish : fish)
    {
        if(isEaten(fish.getCenter()))
        {
            fish.die();
        }
    }
}
void GameRunner::render()
{
    for(auto &eater : eaters)
    {
        eater.render(window);
    }
    for(auto &fish : fish)
    {
        fish.render(window);
    }
}