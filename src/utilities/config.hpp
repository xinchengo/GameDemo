#pragma once

#include<SFML/Graphics.hpp>

#include<array>
#include<string>

namespace TSWF_CONST
{
    constexpr float PI = 3.1415926535897932384626f;

    constexpr float ANG_EPS = PI / 1000;

    constexpr float FLOAT_EPS = 1e-6f;
}

class Config
{
public:
    unsigned int gameFramerateLimit;
    sf::Keyboard::Key gamePauseKey;
    sf::Keyboard::Key gameToggleFullscreenKey;
    float gameDelayWhenGameIsOver;
    unsigned int gameWindowWidth;
    unsigned int gameWindowHeight;
    std::string gameWindowTitle;
    float gameScale;
    bool gameApplyScaleInJSON;

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
    unsigned int swarmFishColor;

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