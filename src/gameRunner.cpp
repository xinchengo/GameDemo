#include <cfloat>
#include <algorithm>

#include "gameRunner.hpp"
#include "utils.hpp"
#include "constants.hpp"

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
bool GameRunner::exceedBoundary(sf::Vector2f pos)
{
    return (pos.x < 0.0f || pos.x > width || pos.y < 0.0f || pos.y > height);
}
void GameRunner::updateSensoryState()
{
    for(auto &fish : allTheFish)
    {
        fish.sensory.lidar.fill(INFINITY);

        for(size_t i=0; i<CONST::LIDAR_CNT; i++)
        {
            auto v = rotate(fish.getVelocity(), CONST::LIDAR_DIRECTIONS[i]);
            float arg = std::atan2(v.y, v.x);
            if(std::abs(arg) < CONST::PI / 2 - CONST::ANG_EPS)
            {
                fish.sensory.lidar[i] = std::min(fish.sensory.lidar[i],
                    (width - fish.getCenter().x) / std::cos(arg));
            }
            if(std::abs(arg) > CONST::PI / 2 + CONST::ANG_EPS)
            {
                fish.sensory.lidar[i] = std::min(fish.sensory.lidar[i],
                    fish.getCenter().x / std::cos(CONST::PI - arg));
            }
            if(CONST::ANG_EPS < arg && arg < CONST::PI - CONST::ANG_EPS)
            {
                fish.sensory.lidar[i] = std::min(fish.sensory.lidar[i],
                    (height - fish.getCenter().y) / std::sin(arg));
            }
            if(-CONST::ANG_EPS > arg && arg > -CONST::PI + CONST::ANG_EPS)
            {
                fish.sensory.lidar[i] = std::min(fish.sensory.lidar[i],
                    fish.getCenter().y / std::sin(-arg));
            }
        }

        for(auto &eater : eaters)
        {
            for(size_t i=0; i<CONST::LIDAR_CNT; i++)
            {
                auto v = rotate(fish.getVelocity(), CONST::LIDAR_DIRECTIONS[i]);
                fish.sensory.lidar[i] = std::min(fish.sensory.lidar[i],
                    disVecCirc(v, eater.getCenter() - fish.getCenter(), eater.getRadius()));
            }
        }
    }
}

GameRunner::GameRunner(float width, float height) : width(width), height(height)
{
    frameNumber = 0;
}
RenderedGameRunner::RenderedGameRunner(sf::RenderWindow &window) : 
    GameRunner((float)window.getSize().x, (float)window.getSize().y),
    window(window) {}
void GameRunner::createRandomFish(int cnt)
{
    for(int i=0; i<30; i++)
    {
        allTheFish.emplace_back(Fish(
            sf::Vector2f(randBetween(0.0f, width), randBetween(0.0f, height))));
    }
    updateSensoryState();
}
void GameRunner::step()
{
    frameNumber++;
    // Update the fish's velocities according to their strategies
    for(auto &fish : allTheFish)
    {
        fish.updateVelocity();
    }
    // If frameNumber satisfies a certain condition, create a new CircularEater
    if(frameNumber % 240 == 0 && eaters.size() <= 20)
    {
        eaters.emplace_back(CircularEater(
            sf::Vector2f(randBetween(0.0f, width), randBetween(0.0f, height))));
    }
    // the fish move
    for(auto &fish : allTheFish)
    {
        fish.step();
    }
    // the eaters enlarge
    for(auto &eater : eaters)
    {
        eater.step();
    }
    // Remove all the fish been eaten
    for(auto &fish : allTheFish)
    {
        if(isEaten(fish.getCenter()) || exceedBoundary(fish.getCenter()))
        {
            fish.die(frameNumber);
        }
    }
    // Update the sensory states of all the fish
    updateSensoryState();
}
void RenderedGameRunner::render()
{
    for(auto &eater : eaters)
    {
        eater.render(window);
    }
    for(auto &fish : allTheFish)
    {
        if(!fish.isDead(frameNumber))
        {
            fish.render(window);
        }
    }
}