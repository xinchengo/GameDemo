#include "strategies.hpp"

LinearStrategy::LinearStrategy()
{
    for(auto &x : a)
        x = randBetween(-0.1f / CONST::LIDAR_CNT, 0.1f / CONST::LIDAR_CNT);
    for(auto &x : b)
        x = randBetween(-0.1f / CONST::LIDAR_CNT, 0.1f / CONST::LIDAR_CNT);
    c = randBetween(-0.1f, 0.1f);
    acceleration_bias = randBetween(-0.1f, 0.1f);
}
void LinearStrategy::mutate()
{
    for(auto &x : a)
    {
        x = ::mutate(x);
    }
    for(auto &x : b)
    {
        x = ::mutate(x);
    }
    c = ::mutate(c);
    acceleration_bias = ::mutate(acceleration_bias);
}
sf::Vector2f LinearStrategy::predictVelocity(SensoryState &sense, sf::Vector2f velocity)
{
    auto speed = std::hypot(velocity.x, velocity.y);
    
    float prod1 = 0, prod2 = 0;
    for(size_t i=0; i<CONST::LIDAR_CNT; i++)
        prod1 += a[i] * sense.lidar[i], prod2 += b[i] * sense.lidar[i];
    prod1 += speed * c;
    prod1 += acceleration_bias;

    float nv = CONST::FISH_SPEED_CHANGE_MAX * std::tanh(prod1);
    float no = CONST::FISH_DIRRECTION_CHANGE_MAX * std::tanh(prod2);
    
    nv = std::clamp(speed + nv, CONST::FISH_SPEED_MIN, CONST::FISH_SPEED_MAX);
    
    return rotate((nv / speed) * velocity, no);
}

sf::Vector2f BaselineStrategy::predictVelocity(SensoryState &sense, sf::Vector2f velocity)
{
    // The function works only when the fish has a 8-ray lidar
    static_assert(CONST::LIDAR_CNT == 8);

    auto speed = std::hypot(velocity.x, velocity.y);

    float intended_acceleration = 0.0f;
    float intended_rotation = 0.0f;

    // If the fish is heading towards an obstacle, slow down
    if(sense.lidar[3] < 100.0f || sense.lidar[4] < 100.0f)
    {
        intended_acceleration = -1.0f * CONST::FISH_SPEED_CHANGE_MAX;
    }
    else // Elsewise, go as fast as it could
    {
        intended_acceleration = 1.0f * CONST::FISH_SPEED_CHANGE_MAX;
    }

    // Rotate toward the direction where the obstacle is furthest away
    float max_weighted_distance = 0.0f;
    size_t index_of_max_distance = 0;
    for(size_t i=0; i<8; i++)
    {
        float weighted_distance = 0.15 * sense.lidar[(i-1)%8]
            + 0.7 * sense.lidar[i] + 0.15 * sense.lidar[(i+1)%8];
        if(weighted_distance > max_weighted_distance)
        {
            index_of_max_distance = i;
            max_weighted_distance = weighted_distance;
        }
    }
    if(index_of_max_distance < 4) // Turn left
    {
        intended_rotation = -1.0f * CONST::FISH_DIRRECTION_CHANGE_MAX;
    }
    else
    {
        intended_rotation = 1.0f * CONST::FISH_DIRRECTION_CHANGE_MAX;
    }

    auto new_speed = std::clamp(speed + intended_acceleration, CONST::FISH_SPEED_MIN, CONST::FISH_SPEED_MAX);
    return rotate((new_speed / speed) * velocity, intended_rotation);
}