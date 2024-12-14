#include "Swarm.hpp"

Boid::Boid() {}
Boid::Boid(sf::Vector2f bounds)
{
    setCenter(randPointInScreen(bounds.x, bounds.y));
    setRandomVelocity(CONST::FLOAT_EPS);
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
    updateAfterTimeHasElaped(CONST::SWARM_STEP_TIME);
}

const std::vector<Boid &> Swarm::extractNearbyBoids(Boid &center)
{
    std::vector<Boid &> nearbyBoids;

    for(auto &boid : boids)
    {
        if(std::addressof(boid) != std::addressof(center) // boid != center
            && dis2(boid.center, center.center) < CONST::SWARM_VISUAL_RANGE)
        {
            nearbyBoids.emplace_back(boid);
        }
    }
    
    return nearbyBoids;
}

void Swarm::updateAcceleration()
{
    clearAcceleration();
    for(auto &boid : boids)
    {
        auto nearbyBoids = extractNearbyBoids(boid);

        avoidOthers(boid, nearbyBoids);
        matchVelocity(boid, nearbyBoids);
        flyTowardsCenter(boid, nearbyBoids);
        clampAcceleration(boid);
        
        // Constrain a boid within the canvas, if a boid gets too close to the edge, nudge it back
        keepWithinBounds(boid);
    }
}

void Swarm::updateAfterTimeHasElaped(float delta)
{
    for(auto &boid : boids)
    {
        boid.velocity = clampVec(boid.velocity + boid.acceleration * delta, CONST::SWARM_SPEED_LIMIT);
        boid.center = boid.center + boid.velocity * delta;
    }
}

Swarm::Swarm(float width, float height) : width(width), height(height)
{
}

void Swarm::clearAcceleration()
{
    for(auto &boid : boids)
        boid.acceleration.x = boid.acceleration.y = 0;
}

void Swarm::avoidOthers(Boid &boid, std::vector<Boid &> nearbyBoids)
{
    sf::Vector2f repulsionSum;
    for(Boid &neighbor : nearbyBoids)
    {
        if(boid.center == neighbor.center)
        {
            repulsionSum += randVecWithLength(CONST::SWARM_SEPARATION_FORCE_WHEN_POINTS_OVERLAP);
        }
        else
        {
            repulsionSum += normalizeVec(boid.center - neighbor.center,
                1.0f / dis2(boid.center, neighbor.center));
        }
    }
    repulsionSum /= float(nearbyBoids.size());
    boid.acceleration += repulsionSum * CONST::SWARM_SEPARATION_FACTOR;
}

void Swarm::matchVelocity(Boid &boid, std::vector<Boid &> nearbyBoids)
{
    sf::Vector2f alignmentSum;
    for(Boid &neighbor : nearbyBoids)
    {
        alignmentSum += (neighbor.velocity - boid.velocity);
    }
    alignmentSum /= float(nearbyBoids.size());
    boid.acceleration += alignmentSum * CONST::SWARM_ALIGNMENT_FACTOR;
}

void Swarm::flyTowardsCenter(Boid &boid, std::vector<Boid &> nearbyBoids)
{
    sf::Vector2f centerOfMass;
    for(Boid &neighbor : nearbyBoids)
    {
        centerOfMass += neighbor.center;
    }
    centerOfMass /= float(nearbyBoids.size());
    boid.acceleration += (centerOfMass - boid.center) * CONST::SWARM_COHESION_FACTOR;
}

void Swarm::clampAcceleration(Boid &boid)
{
    boid.acceleration = clampVec(boid.acceleration, CONST::SWARM_ACCELERATION_LIMIT);
}

void Swarm::keepWithinBounds(Boid &boid)
{
    if(boid.center.x < CONST::SWARM_EDGE_RANGE)
    {
        boid.acceleration.x += CONST::SWARM_TURN_FACTOR;
    }
    else if(boid.center.x > width - CONST::SWARM_EDGE_RANGE)
    {
        boid.acceleration.x -= CONST::SWARM_TURN_FACTOR;
    }
    if(boid.center.y < CONST::SWARM_EDGE_RANGE)
    {
        boid.acceleration.y += CONST::SWARM_TURN_FACTOR;
    }
    else if(boid.center.y > height - CONST::SWARM_EDGE_RANGE)
    {
        boid.acceleration.y -= CONST::SWARM_TURN_FACTOR;
    }
}
