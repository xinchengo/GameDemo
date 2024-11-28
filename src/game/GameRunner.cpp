#include <memory>

#include "GameRunner.hpp"
#include "utilities/utils.hpp"
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
CircularEater* GameRunner::createCircularEater(sf::Vector2f pos)
{
    return new CircularEater(pos);
}
GreenCircle* GameRunner::createGreenCircle(sf::Vector2f pos)
{
    return new GreenCircle(pos);
}
bool GameRunner::isEaten(sf::Vector2f pos)
{
    for(auto &eater : eaters)
    {
        if(dis2(eater->getCenter(), pos) < eater->getRadius())
        {
            return true;
        }
    }
    return false;
}
uint8_t GameRunner::exceedBoundary(sf::Vector2f pos)
{
    return (pos.x < 0.0f | pos.x > width) | ((pos.y < 0.0f || pos.y > height) << 1);
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

    for(auto &eater : eaters)
    {
        for(size_t i=0; i<CONST::LIDAR_CNT; i++)
        {
            auto v = rotate(fish->getVelocity(), CONST::LIDAR_DIRECTIONS[i]);
            fish->sensory.lidar[i] = std::min(fish->sensory.lidar[i],
                disVecCirc(v, eater->getCenter() - fish->getCenter(), eater->getRadius()));
        }
    }
}

GameRunner::GameRunner(float width, float height) : width(width), height(height), snake(200)
{
    clear();
}

void GameRunner::clear()
{
    frameNumber = 0;
    eaters.clear();
    fishes.clear();
}
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
void GameRunner::step()
{
    frameNumber++;
    // Update the fish's velocities according to their strategies
    for(auto &fish : fishes)
    {
        fish->updateVelocity();
    }
    // If frameNumber satisfies a certain condition, create a new CircularEater
    if(frameNumber % 240 == 0 && eaters.size() <= 20)
    {
        eaters.emplace_back(createCircularEater(randPointInScreen(width, height)));
    }
    // the fish move
    for(auto &fish : fishes)
    {
        fish->step();
    }
    // the snake moves
    snake.step();
    // the eaters moves and enlarges
    for(auto &eater : eaters)
    {
        eater->step();
        // the eaters bounce on hitting the boundary
        eater->bounce(exceedBoundary(eater->getCenter()));
    }
    // The greenCircle moves
    for(auto &circ : greenCircles)
    {
        circ->step();
        circ->bounce(exceedBoundary(circ->getCenter()));
    }
    // Remove all the fish been eaten
    for(auto &fish : fishes)
    {
        if(isEaten(fish->getCenter()) || exceedBoundary(fish->getCenter()))
        {
            fish->die(frameNumber);
        }
    }
    // Update the sensory states of all the fish
    for(auto &fish : fishes)
    {
        updateSensoryState(fish);
    }
}
bool GameRunner::fishAllDead()
{
    for(auto &fish : fishes)
    {
        if(!fish->isDead(frameNumber))
        {
            return false;
        }
    }
    return true;
}

HeadlessGameRunner::HeadlessGameRunner(float width, float height) : GameRunner(width, height) {}
// TrainStats HeadlessGameRunner::train(std::vector<LinearStrategy> &strategies)
// {
//     clear();

//     for(auto &strategy : strategies)
//     {
//         newRandomFish(strategy);
//     }
//     do
//     {
//         step();
//     } while (!fishAllDead());
    
//     TrainStats output;

//     std::vector<size_t> lifespan(strategies.size());
//     for(size_t i=0; i<strategies.size(); i++)
//     {
//         lifespan[i] = fishes[i]->timeOfDeath();
//     }
//     output.lifespans.assign(lifespan.begin(), lifespan.end());
//     std::partial_sum(lifespan.begin(), lifespan.end(), lifespan.begin());
//     output.meanLifespan = (float)lifespan.back() / strategies.size();

//     for(size_t i=0; i<strategies.size(); i++)
//     {
//         auto ind = randint((size_t)1, lifespan.back());
//         auto k = std::lower_bound(lifespan.begin(), lifespan.end(), ind) - lifespan.begin();
//         output.yieldingStrategies.emplace_back(fishes[k]->strategy);
//     }

//     // Mutate the strategies

//     for(auto &strategy : output.yieldingStrategies)
//     {
//         strategy.mutate();
//     }

//     return output;
// }