#pragma once

#include<array>

namespace CONST
{
    constexpr float PI = 3.1415926535897932384626f;

    constexpr float ANG_EPS = PI / 1000;
    
    constexpr std::array<float, 8> LIDAR_DIRECTIONS = 
        {-PI*7.0f/8.0f, -PI*5.0f/8.0f, -PI*3.0f/8.0f, -PI*1.0f/8.0f,
        PI*1.0f/8.0f, PI*3.0f/8.0f, PI*5.0f/8.0f, PI*7.0f/8.0f};
}