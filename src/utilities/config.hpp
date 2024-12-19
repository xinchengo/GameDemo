#pragma once

#include<SFML/Graphics.hpp>

#include<array>
#include<string>

namespace CONST
{
    constexpr float PI = 3.1415926535897932384626f;

    constexpr float ANG_EPS = PI / 1000;

    constexpr float FLOAT_EPS = 1e-6f;

    constexpr unsigned int GAME_FRAMERATE_LIMIT = 60U;

    constexpr float SWARM_SEPARATION_FACTOR = 180.0f;

    constexpr float SWARM_ALIGNMENT_FACTOR = 0.6f;

    constexpr float SWARM_COHESION_FACTOR = 7.2f;

    constexpr float SWARM_TURN_FACTOR = 360.0f;

    constexpr float SWARM_PREDATOR_REPLUSION_FACTOR = 180.0f;

    constexpr float SWARM_VISUAL_RANGE = 75.0f;

    constexpr float SWARM_AVOID_THRESHOLD_RANGE = 20.0f;

    constexpr float SWARM_SPEED_LIMIT = 180.0f;

    constexpr float SWARM_EDGE_RANGE = 100.0f;

    constexpr float SNAKE_SPEED = 120.0f;

    constexpr float SNAKE_CIRCLE_SIZE = 20.f;

    constexpr float SNAKE_SEGMENT_SPACING = 20.f;

    constexpr float SNAKE_GROWTH_PER_FISH = 0.25f;

    constexpr float SNAKE_MAX_TURN_ANGLE = 3.f;

    constexpr float SNAKE_COEFFICIENT_OF_PREDATOR_MODE = 0.8f;

    constexpr unsigned int SNAKE_POLYGON_INDICATOR_COLOR = 0x5b9bd5ff;

    constexpr float GREEN_CIRCLE_SPEED_MIN = 10.0f;

    constexpr float GREEN_CIRCLE_SPEED_MAX = 60.0f;

    constexpr float GREEN_CIRCLE_RADIUS = 20.0f;

    constexpr float GREEN_CIRCLE_OUTLINE_THICKNESS = 5.0f;
}