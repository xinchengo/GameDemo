#include "Swarm.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

Boid::Boid() {}
Boid::Boid(sf::Vector2f bounds)
{
    setCenter(randPointInScreen(bounds.x, bounds.y));
    setRandomVelocity(randBetween(0.0f, 10.0f * config.swarmSpeedLimit));
}

void Swarm::createBoids(int cnt)
{
    for(int i=1; i<=cnt; i++)
        boids.emplace_back(sf::Vector2f(width, height));
}

const std::vector<Boid> &Swarm::getBoids()
{
    return boids;
}

int Swarm::removeBoidsEaten(const std::function<bool(sf::Vector2f)>& hasEaten)
{
    // Partition boids into active and eaten
    auto partitionIt = std::partition(boids.begin(), boids.end(),
        [&](const Boid& boid) { return !hasEaten(boid.center); });

    // Calculate number of removed boids
    int removedCount = static_cast<int>(std::distance(partitionIt, boids.end()));

    // Move eaten boids to deadBoids
    deadBoids.insert(deadBoids.end(),
        std::make_move_iterator(partitionIt),
        std::make_move_iterator(boids.end()));

    // Erase eaten boids from boids vector
    boids.erase(partitionIt, boids.end());

    // Remove dead boids with negative disappearTime
    deadBoids.erase(std::remove_if(deadBoids.begin(), deadBoids.end(),
        [&](const DeadBoid& deadBoid) { return deadBoid.disappearTime < 0.0f; }),
        deadBoids.end());

    return removedCount;
}

void Swarm::setPredators(std::vector<sf::Vector2f> &predatorList)
{
    predators = predatorList;
}

void Swarm::step(float time)
{
    updateAcceleration(time);
    decrementDeadBoids(time);
}

void Swarm::render(sf::RenderWindow &window)
{
    sf::VertexArray tri(sf::Triangles);

    sf::Color boidColor(config.swarmFishColor);

    for(auto &boid : boids)
    {
        sf::Vector2f p1 = boid.getCenter(), v = boid.getVelocity();
        if(v == sf::Vector2f(0.0f, 0.0f))
            v = sf::Vector2f(1.0f, 0.0f);
        sf::Vector2f p2 = p1 - rotate(normalizeVec(v, 15), TSWF_CONST::PI/18); 
        sf::Vector2f p3 = p1 - rotate(normalizeVec(v, 15), -TSWF_CONST::PI/18);

        tri.append(sf::Vertex(p1, boidColor));
        tri.append(sf::Vertex(p2, boidColor));
        tri.append(sf::Vertex(p3, boidColor));
    }

    window.draw(tri);

    tri.clear();
    
    for(auto &deadBoid : deadBoids)
    {
        sf::Vector2f p1 = deadBoid.getCenter(), v = deadBoid.getVelocity();
        if(v == sf::Vector2f(0.0f, 0.0f))
            v = sf::Vector2f(1.0f, 0.0f);
        sf::Vector2f p2 = p1 - rotate(normalizeVec(v, 15), TSWF_CONST::PI/18); 
        sf::Vector2f p3 = p1 - rotate(normalizeVec(v, 15), -TSWF_CONST::PI/18);

        tri.append(sf::Vertex(p1, sf::Color::White));
        tri.append(sf::Vertex(p2, sf::Color::White));
        tri.append(sf::Vertex(p3, sf::Color::White));
    }

    window.draw(tri);
}

const std::vector<std::reference_wrapper<Boid>> Swarm::extractNearbyBoids(Boid &center, float dis)
{
    std::vector<std::reference_wrapper<Boid>> nearbyBoids;

    for(auto &boid : boids)
    {
        if(std::addressof(boid) != std::addressof(center) // boid != center
            && dis2(boid.center, center.center) < dis)
        {
            nearbyBoids.emplace_back(boid);
        }
    }
    
    return nearbyBoids;
}

void Swarm::updateAcceleration(float time)
{
    for(auto &boid : boids)
    {
        auto nearbyBoids = extractNearbyBoids(boid, config.swarmVisualRange);
        auto boidsToAvoid = extractNearbyBoids(boid, config.swarmAvoidThresholdRange);

        flyTowardsCenter(boid, nearbyBoids, time);
        avoidOthers(boid, boidsToAvoid, time);
        matchVelocity(boid, nearbyBoids, time);
        avoidPredators(boid, time);
        clampVelocity(boid);
        keepWithinBounds(boid, time);

        boid.center = boid.center + boid.velocity * time;
    }
}

void Swarm::decrementDeadBoids(float time)
{
    for(auto &deadBoid : deadBoids)
    {
        deadBoid.disappearTime -= time;
    }
}

Swarm::Swarm(float width, float height) : width(width), height(height)
{
}

void Swarm::avoidOthers(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids, float time)
{
    sf::Vector2f repulsionSum;
    for(Boid &neighbor : nearbyBoids)
    {
        repulsionSum += boid.center - neighbor.center;
    }
    // repulsionSum /= float(nearbyBoids.size());
    boid.velocity += repulsionSum * config.swarmSeparationFactor * time;
}

void Swarm::avoidPredators(Boid &boid, float time)
{
    sf::Vector2f replusionSum;
    for(auto &predator : predators)
    {
        if(dis2(predator, boid.center) < config.swarmVisualRange)
        {
            replusionSum += boid.center - predator;
        }
    }
    boid.velocity += replusionSum * config.swarmPredatorReplusionFactor * time;
}

void Swarm::matchVelocity(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids, float time)
{
    sf::Vector2f averageVelocity;
    for(Boid &neighbor : nearbyBoids)
    {
        averageVelocity += neighbor.velocity;
    }
    if(nearbyBoids.empty() == false)
        averageVelocity /= float(nearbyBoids.size());
    boid.velocity += (averageVelocity - boid.velocity) * config.swarmAlignmentFactor * time;
}

void Swarm::flyTowardsCenter(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids, float time)
{
    sf::Vector2f centerOfMass;
    for(Boid &neighbor : nearbyBoids)
    {
        centerOfMass += neighbor.center;
    }
    if(nearbyBoids.empty() == false)
        centerOfMass /= float(nearbyBoids.size());
    boid.velocity += (centerOfMass - boid.center) * config.swarmCohesionFactor * time;
}

void Swarm::clampVelocity(Boid &boid)
{
    boid.velocity = clampVec(boid.velocity, config.swarmSpeedLimit);
}

void Swarm::keepWithinBounds(Boid &boid, float time)
{
    if(boid.center.x < config.swarmEdgeRange)
    {
        boid.velocity.x += config.swarmTurnFactor * time;
    }
    else if(boid.center.x > width - config.swarmEdgeRange)
    {
        boid.velocity.x -= config.swarmTurnFactor * time;
    }
    if(boid.center.y < config.swarmEdgeRange)
    {
        boid.velocity.y += config.swarmTurnFactor * time;
    }
    else if(boid.center.y > height - config.swarmEdgeRange)
    {
        boid.velocity.y -= config.swarmTurnFactor * time;
    }
}

DeadBoid::DeadBoid() : Boid(), disappearTime(config.swarmDisappearTimeAfterEaten) {}

DeadBoid::DeadBoid(const Boid &boid) : Boid(boid), disappearTime(config.swarmDisappearTimeAfterEaten) {}