#include "Swarm.hpp"

#include <iostream>

Boid::Boid() {}
Boid::Boid(sf::Vector2f bounds)
{
    setCenter(randPointInScreen(bounds.x, bounds.y));
    setRandomVelocity(randBetween(-10.0f, 10.0f));
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

void Swarm::step()
{
    updateAcceleration();
}

void Swarm::render(sf::RenderWindow &window)
{
    sf::VertexArray tri(sf::Triangles);

    // std::cout << boids.front().getCenter().x << std::endl;

    for(auto &boid : boids)
    {
        sf::Vector2f p1 = boid.getCenter(), v = boid.getVelocity();
        if(v == sf::Vector2f(0.0f, 0.0f))
            v = sf::Vector2f(1.0f, 0.0f);
        sf::Vector2f p2 = p1 - rotate(normalizeVec(v, 15), CONST::PI/18); 
        sf::Vector2f p3 = p1 - rotate(normalizeVec(v, 15), -CONST::PI/18);

        tri.append(sf::Vertex(p1, sf::Color::Red));
        tri.append(sf::Vertex(p2, sf::Color::Red));
        tri.append(sf::Vertex(p3, sf::Color::Red));
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

void Swarm::updateAcceleration()
{
    for(auto &boid : boids)
    {
        auto nearbyBoids = extractNearbyBoids(boid, CONST::SWARM_VISUAL_RANGE);
        auto boidsToAvoid = extractNearbyBoids(boid, CONST::SWARM_AVOID_THRESHOLD_RANGE);

        flyTowardsCenter(boid, nearbyBoids);
        avoidOthers(boid, boidsToAvoid);
        matchVelocity(boid, nearbyBoids);
        clampVelocity(boid);
        keepWithinBounds(boid);

        boid.center = boid.center + boid.velocity;
    }
}

Swarm::Swarm(float width, float height) : width(width), height(height)
{
}

void Swarm::avoidOthers(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids)
{
    sf::Vector2f repulsionSum;
    for(Boid &neighbor : nearbyBoids)
    {
        repulsionSum += boid.center - neighbor.center;
    }
    // repulsionSum /= float(nearbyBoids.size());
    boid.velocity += repulsionSum * CONST::SWARM_SEPARATION_FACTOR;
}

void Swarm::matchVelocity(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids)
{
    sf::Vector2f averageVelocity;
    for(Boid &neighbor : nearbyBoids)
    {
        averageVelocity += neighbor.velocity;
    }
    if(nearbyBoids.empty() == false)
        averageVelocity /= float(nearbyBoids.size());
    boid.velocity += (averageVelocity - boid.velocity) * CONST::SWARM_ALIGNMENT_FACTOR;
}

void Swarm::flyTowardsCenter(Boid &boid, std::vector<std::reference_wrapper<Boid>> &nearbyBoids)
{
    sf::Vector2f centerOfMass;
    for(Boid &neighbor : nearbyBoids)
    {
        centerOfMass += neighbor.center;
    }
    if(nearbyBoids.empty() == false)
        centerOfMass /= float(nearbyBoids.size());
    boid.velocity += (centerOfMass - boid.center) * CONST::SWARM_COHESION_FACTOR;
}

void Swarm::clampVelocity(Boid &boid)
{
    boid.velocity = clampVec(boid.velocity, CONST::SWARM_SPEED_LIMIT);
}

void Swarm::keepWithinBounds(Boid &boid)
{
    if(boid.center.x < CONST::SWARM_EDGE_RANGE)
    {
        boid.velocity.x += CONST::SWARM_TURN_FACTOR;
    }
    else if(boid.center.x > width - CONST::SWARM_EDGE_RANGE)
    {
        boid.velocity.x -= CONST::SWARM_TURN_FACTOR;
    }
    if(boid.center.y < CONST::SWARM_EDGE_RANGE)
    {
        boid.velocity.y += CONST::SWARM_TURN_FACTOR;
    }
    else if(boid.center.y > height - CONST::SWARM_EDGE_RANGE)
    {
        boid.velocity.y -= CONST::SWARM_TURN_FACTOR;
    }
}
