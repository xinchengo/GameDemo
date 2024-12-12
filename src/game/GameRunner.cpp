#include <memory>

#include "GameRunner.hpp"
#include "utilities/mathUtils.hpp"
#include "utilities/config.hpp"

Fish* GameRunner::createFish(sf::Vector2f pos, CONST::FISH_STRATEGY stra)
{
    switch(stra)
    {
        case CONST::FISH_STRATEGY::LINEAR:
            return new Fish(pos, std::make_unique<LinearStrategy>());
        case CONST::FISH_STRATEGY::BASELINE:
            return new Fish(pos, std::make_unique<BaselineStrategy>());
        default:
            return nullptr;
    }
}
Fish* GameRunner::createFish(sf::Vector2f pos, std::unique_ptr<FishStrategy> &stra)
{
    return new Fish(pos, std::move(stra));
}
GreenCircle* GameRunner::createGreenCircle(sf::Vector2f pos)
{
    return new GreenCircle(pos);
}
uint8_t GameRunner::exceedBoundary(sf::Vector2f pos)
{
    return (pos.x < 0.0f || pos.x > width) | ((pos.y < 0.0f || pos.y > height) << 1);
}
void GameRunner::updateSensoryState(std::unique_ptr<Fish> &fish)
{
    if(fish->isDead(frameNumber))
    {
        return;
    }

    fish->sensory.lidar.fill(INFINITY);

    for(size_t i=0; i<CONST::LIDAR_CNT; i++)
    {
        auto v = rotate(fish->getVelocity(), CONST::LIDAR_DIRECTIONS[i]);
        float arg = std::atan2(v.y, v.x);
        if(std::abs(arg) < CONST::PI / 2 - CONST::ANG_EPS)
        {
            fish->sensory.lidar[i] = std::min(fish->sensory.lidar[i],
                (width - fish->getCenter().x) / std::cos(arg));
        }
        if(std::abs(arg) > CONST::PI / 2 + CONST::ANG_EPS)
        {
            fish->sensory.lidar[i] = std::min(fish->sensory.lidar[i],
                fish->getCenter().x / std::cos(CONST::PI - arg));
        }
        if(CONST::ANG_EPS < arg && arg < CONST::PI - CONST::ANG_EPS)
        {
            fish->sensory.lidar[i] = std::min(fish->sensory.lidar[i],
                (height - fish->getCenter().y) / std::sin(arg));
        }
        if(-CONST::ANG_EPS > arg && arg > -CONST::PI + CONST::ANG_EPS)
        {
            fish->sensory.lidar[i] = std::min(fish->sensory.lidar[i],
                fish->getCenter().y / std::sin(-arg));
        }
    }
}

GameRunner::GameRunner(float width, float height) : width(width), height(height), snake(), frameNumber(0) { }

void GameRunner::newRandomFish(CONST::FISH_STRATEGY stra, int cnt)
{
    for(int i=0; i<cnt; i++)
    {
        fishes.emplace_back(createFish(randPointInScreen(width, height), stra));
        updateSensoryState(fishes.back());
    }
}
void GameRunner::newRandomFish(std::unique_ptr<FishStrategy> &stra)
{
    fishes.emplace_back(createFish(randPointInScreen(width, height), stra));
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
    // Update the fish's velocities according to their strategies
    for(auto &fish : fishes)
    {
        fish->updateVelocity();
    }
    // the fish move
    for(auto &fish : fishes)
    {
        fish->step();
    }
    // the snake moves
    if(snake) // if the snake exists
    {
        snake->step();
    }
    // The greenCircle moves
    for(auto &circ : greenCircles)
    {
        circ->step();
        // the green circle bounces on hitting the boundary
        circ->bounce(exceedBoundary(circ->getCenter()));
    }
    // Remove all the fish been eaten
    if(snake)
    {
        for(size_t i = 0; i < fishes.size(); i++)
        {
            if(snake->hasEaten(fishes[i]->getCenter()))
            {
                // Remove the fish from list
                std::swap(fishes[i], fishes.back());
                fishes.pop_back();
            }
        }
    }
    // Remove all the green circles been eaten
    if(snake)
    {
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
    }
    // Update the sensory states of all the fish
    for(auto &fish : fishes)
    {
        updateSensoryState(fish);
    }
}