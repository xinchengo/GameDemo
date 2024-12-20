#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include <vector>
#include "utilities/properties.hpp"
#include "utilities/config.hpp"


class Boid : public HasCenter, public HasVelocity
{
public:
    Boid();
    /**
     * @brief Construct a new Boid object with random center and velocity
     * within `Bounds`
     */
    Boid(sf::Vector2f bounds);
};

class DeadBoid : public Boid
{
public:
    float disappearTime;
    DeadBoid();
    DeadBoid(const Boid &boid);
};

/**
 * @brief Class for the simulation of the swarm of fish 
 * 
 */
class Swarm
{
private:
    float width, height;

    std::vector<Boid> boids;
    std::vector<DeadBoid> deadBoids;
    std::vector<sf::Vector2f> predators;
    
    void avoidOthers(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids, float time);
    void avoidPredators(Boid &boid, float time);
    void matchVelocity(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids, float time);
    void flyTowardsCenter(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids, float time);
    /// @brief Helper function to nudge a boid back if it gets too close to the edge
    void clampVelocity(Boid &boid);
    void keepWithinBounds(Boid& boid, float time);
    /**
     * @brief Helper function to extract all the boids within `dis` of `center`
     * 
     * @param x the center
     * @return a std::vector<Boid&> storing all the boids within
     * `CONST::SWARM_VISUAL_RANGE` of center.
     */
    const std::vector<std::reference_wrapper<Boid>> extractNearbyBoids(Boid &center, float dis);
    /// @brief Helper function to update the acceleration of all the boids.
    void updateAcceleration(float time);
    /// @brief Decrement all `disappearTime` of all dead boids
    void decrementDeadBoids(float time);
public:
    Swarm(float width, float height);
    /**
     * @brief Create `cnt` `Boid`s
     * 
     * @param cnt number of `Boid`s to generate
     */
    void createBoids(int cnt);
    const std::vector<Boid> &getBoids();
    /**
     * @brief Remove all the boids that have been eaten
     * according to function `hasEaten`.
     * 
     * @param hasEaten the criteria of being eaten
     * @return the number of boids eaten
     */
    int removeBoidsEaten(const std::function<bool(sf::Vector2f)> &hasEaten);
    /**
     * @brief Set `predators` as the list of predators
     * 
     * @param predators the list of predators
     */
    void setPredators(std::vector<sf::Vector2f> &predators);
    void step(float time);
    void render(sf::RenderWindow &window);
};