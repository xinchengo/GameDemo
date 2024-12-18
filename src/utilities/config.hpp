#pragma once

#include<array>
#include<string>

namespace CONST
{
    constexpr size_t FRAME_CNT_INFINITY = -1;

    constexpr float PI = 3.1415926535897932384626f;

    constexpr float ANG_EPS = PI / 1000;

    constexpr float FLOAT_EPS = 1e-6f;

    constexpr float SWARM_SEPARATION_FACTOR = 0.05f;

    constexpr float SWARM_ALIGNMENT_FACTOR = 0.01f;

    constexpr float SWARM_COHESION_FACTOR = 0.002f;

    constexpr float SWARM_TURN_FACTOR = 0.1f;

    constexpr float SWARM_PREDATOR_REPLUSION_FACTOR = 0.05f;

    constexpr float SWARM_VISUAL_RANGE = 75.0f;

    constexpr float SWARM_AVOID_THRESHOLD_RANGE = 20.0f;

    constexpr float SWARM_SPEED_LIMIT = 3.0f;

    constexpr float SWARM_EDGE_RANGE = 100.0f;

    constexpr float SNAKE_SPEED = 2.0f;

    constexpr float SNAKE_CIRCLE_SIZE = 20.f;

    constexpr float SNAKE_COEFFICIENT_OF_PREDATOR_MODE = 0.8f;

    constexpr float GREEN_CIRCLE_SPEED_MIN = 0.10f;

    constexpr float GREEN_CIRCLE_SPEED_MAX = 1.0f;

    constexpr float GREEN_CIRCLE_RADIUS = 20.0f;

    constexpr float GREEN_CIRCLE_OUTLINE_THICKNESS = 5.0f;

    constexpr   float GROWTH_PERCENTAGE = 0.004f;
}