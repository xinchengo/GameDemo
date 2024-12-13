#pragma once

#include<SFML/Graphics.hpp>

#include<array>
#include<string>

namespace CONST
{
    constexpr float PI = 3.1415926535897932384626f;

    constexpr float ANG_EPS = PI / 1000;

    constexpr float FLOAT_EPS = 1e-6f;
}

class Config
{
public:
    unsigned int gameFramerateLimit;
    unsigned int gamePauseKey;
    float gameDelayWhenGameIsOver;

    float swarmSeparationFactor;
    float swarmAlignmentFactor;
    float swarmCohesionFactor;
    float swarmTurnFactor;
    float swarmPredatorReplusionFactor;
    float swarmVisualRange;
    float swarmAvoidThresholdRange;
    float swarmSpeedLimit;
    float swarmEdgeRange;
    float swarmDisappearTimeAfterEaten;

    float snakeSpeed;
    float snakeCircleSize;
    float snakeSegmentSpacing;
    float snakeGrowthPerFish;
    float snakeMaxTurnAngle;
    float snakeCoefficientOfPredatorMode;
    unsigned int snakePolygonIndicatorColor;

    float greenCircleSpeedMin;
    float greenCircleSpeedMax;
    float greenCircleRadius;
    float greenCircleOutlineThickness;

    void loadConfig(const std::string& filename);
};

extern Config config;