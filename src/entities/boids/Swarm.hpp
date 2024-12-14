#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
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

/**
 * @brief Class for the simulation of the swarm of fish 
 * 
 */
class Swarm
{
private:
    float width, height;

    std::vector<Boid> boids;
    
    void avoidOthers(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids);
    void matchVelocity(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids);
    void flyTowardsCenter(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids);
    /// @brief Helper function to nudge a boid back if it gets too close to the edge
    void clampVelocity(Boid &boid);
    void keepWithinBounds(Boid& boid);
    /**
     * @brief Helper function to extract all the boids within `dis` of `center`
     * 
     * @param x the center
     * @return a std::vector<Boid&> storing all the boids within
     * `CONST::SWARM_VISUAL_RANGE` of center.
     */
    const std::vector<std::reference_wrapper<Boid>> extractNearbyBoids(Boid &center, float dis);
    /// @brief Helper function to update the acceleration of all the boids.
    void updateAcceleration();
public:
    Swarm(float width, float height);
    /**
     * @brief Create `cnt` `Boid`s
     * 
     * @param cnt number of `Boid`s to generate
     */
    void createBoids(int cnt);
    const std::vector<Boid> &getBoids();
    void step();
    void render(sf::RenderWindow &window);
};