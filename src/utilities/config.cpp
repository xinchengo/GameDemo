#include "config.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

// Global configuration object
Config config;

void Config::loadConfig(const std::string &filename)
{
    std::ifstream file(filename);
    nlohmann::json jsonConfig;
    file >> jsonConfig;

    // Load game settings
    gameFramerateLimit = jsonConfig["game"].value("framerateLimit", gameFramerateLimit);

    // Load swarm settings
    swarmSeparationFactor = jsonConfig["swarm"].value("separationFactor", swarmSeparationFactor);
    swarmAlignmentFactor = jsonConfig["swarm"].value("alignmentFactor", swarmAlignmentFactor);
    swarmCohesionFactor = jsonConfig["swarm"].value("cohesionFactor", swarmCohesionFactor);
    swarmTurnFactor = jsonConfig["swarm"].value("turnFactor", swarmTurnFactor);
    swarmPredatorReplusionFactor = jsonConfig["swarm"].value("predatorRepulsionFactor", swarmPredatorReplusionFactor);
    swarmVisualRange = jsonConfig["swarm"].value("visualRange", swarmVisualRange);
    swarmAvoidThresholdRange = jsonConfig["swarm"].value("avoidThresholdRange", swarmAvoidThresholdRange);
    swarmSpeedLimit = jsonConfig["swarm"].value("speedLimit", swarmSpeedLimit);
    swarmEdgeRange = jsonConfig["swarm"].value("edgeRange", swarmEdgeRange);
    swarmDisappearTimeAfterEaten = jsonConfig["swarm"].value("disappearTimeAfterEaten", swarmDisappearTimeAfterEaten);

    // Load snake settings
    snakeSpeed = jsonConfig["snake"].value("speed", snakeSpeed);
    snakeCircleSize = jsonConfig["snake"].value("circleSize", snakeCircleSize);
    snakeSegmentSpacing = jsonConfig["snake"].value("segmentSpacing", snakeSegmentSpacing);
    snakeGrowthPerFish = jsonConfig["snake"].value("growthPerFish", snakeGrowthPerFish);
    snakeMaxTurnAngle = jsonConfig["snake"].value("maxTurnAngle", snakeMaxTurnAngle);
    snakeCoefficientOfPredatorMode = jsonConfig["snake"].value("coefficientOfPredatorMode", snakeCoefficientOfPredatorMode);
    snakePolygonIndicatorColor = jsonConfig["snake"].value("polygonIndicatorColor", snakePolygonIndicatorColor);

    // Load green circle settings
    greenCircleSpeedMin = jsonConfig["greenCircle"].value("speedMin", greenCircleSpeedMin);
    greenCircleSpeedMax = jsonConfig["greenCircle"].value("speedMax", greenCircleSpeedMax);
    greenCircleRadius = jsonConfig["greenCircle"].value("radius", greenCircleRadius);
    greenCircleOutlineThickness = jsonConfig["greenCircle"].value("outlineThickness", greenCircleOutlineThickness);
}