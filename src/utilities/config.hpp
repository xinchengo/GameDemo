#pragma once

#include<array>
#include<string>

namespace CONST
{
    constexpr size_t FRAME_CNT_INFINITY = -1;

    constexpr float PI = 3.1415926535897932384626f;

    constexpr float ANG_EPS = PI / 1000;
    
    constexpr std::array<float, 8> LIDAR_DIRECTIONS = 
        {-PI*7.0f/8.0f, -PI*5.0f/8.0f, -PI*3.0f/8.0f, -PI*1.0f/8.0f,
        PI*1.0f/8.0f, PI*3.0f/8.0f, PI*5.0f/8.0f, PI*7.0f/8.0f};

    constexpr auto LIDAR_CNT = LIDAR_DIRECTIONS.size();

    constexpr float FISH_SPEED_MIN = 0.05f;

    constexpr float FISH_SPEED_MAX = 0.5f;

    constexpr float FISH_SPEED_CHANGE_MAX = 0.01f;
    
    constexpr float FISH_DIRRECTION_CHANGE_MAX = PI / 20;

    enum FISH_STRATEGY
    {
        LINEAR, BASELINE
    };

    constexpr float FLOAT_EPS = 1e-6f;

    constexpr float SNAKE_SPEED = 2.0f;

    constexpr float SNAKE_CIRCLE_SIZE = 20.f;

    constexpr float SNAKE_COEFFICIENT_OF_PREDATOR_MODE = 0.8f;

    constexpr float GREEN_CIRCLE_SPEED_MIN = 0.10f;

    constexpr float GREEN_CIRCLE_SPEED_MAX = 1.0f;

    constexpr float GREEN_CIRCLE_RADIUS = 20.0f;

    constexpr float GREEN_CIRCLE_OUTLINE_THICKNESS = 5.0f;
}