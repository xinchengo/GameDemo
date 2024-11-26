#pragma once

#include<SFML/Graphics.hpp>
#include<vector>
#include<string>
#include<memory>

#include "entities.hpp"
#include "utils.hpp"
#include "constants.hpp"

class Scene  
{  
public:  
    virtual ~Scene() = default;  

    // Update the scene - called in the main loop  
    virtual void step() = 0;  

    // Render the scene  
    virtual void render() = 0;  

    // Handle user input  
    virtual void handleUserInput() = 0;  
};

class SceneManager
{  
private:  
    std::shared_ptr<Scene> currentScene;  
    sf::RenderWindow &window;  

public:  
    SceneManager(sf::RenderWindow &window) : window(window) {}  

    void setScene(std::shared_ptr<Scene> scene) {  
        currentScene = std::move(scene);
    }  

    void update() {  
        if (currentScene) {  
            currentScene->handleUserInput();  
            currentScene->step();  
            currentScene->render();  
        }  
    }  
};

class GameRunner
{
private:
    uint8_t exceedBoundary(sf::Vector2f);
    void updateSensoryState(std::unique_ptr<Fish> &);
    Fish* createFish(sf::Vector2f, CONST::FISH_STRATEGY);
    Fish* createFish(sf::Vector2f, std::unique_ptr<FishStrategy> &);
    CircularEater* createCircularEater(sf::Vector2f);
    GreenCircle* createGreenCircle(sf::Vector2f);

protected:
    size_t frameNumber;
    float height, width;
    std::vector<std::unique_ptr<CircularEater>> eaters;
    std::vector<std::unique_ptr<Fish>> fishes;
    std::vector<std::unique_ptr<GreenCircle>> greenCircles;
    Snake snake;
    
    // Fish* createFish(sf::Vector2f, std::unique_ptr<FishStrategy>);
    bool isEaten(sf::Vector2f);
    
public:

    void clear();
    void newRandomFish(CONST::FISH_STRATEGY, int=1);
    void newRandomFish(std::unique_ptr<FishStrategy> &);
    void newGreenCircles(int=1);
    GameRunner(float, float);
    void step();
    bool fishAllDead();
};

class RenderedGameRunner : public GameRunner, public Scene
{
private:
    sf::RenderWindow &window;
public:
    void step() override { GameRunner::step(); }
    void render();
    void handleUserInput();
    RenderedGameRunner(sf::RenderWindow &);
};

struct TrainStats
{
    float meanLifespan;
    std::vector<float> lifespans;
    std::vector<LinearStrategy> yieldingStrategies;
};

class HeadlessGameRunner : public GameRunner
{
private:
public:
    HeadlessGameRunner(float, float);
    TrainStats train(std::vector<LinearStrategy> &);
};