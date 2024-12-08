#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <queue>
#include <memory>
#include <variant>

class Scene;

/***
 * @struct SceneEvent
 * @brief SceneEvent is a type of event emitted by a scene.
 * 
 * It allow a scene to communicate with the SceneManager to
 * perform scene-switching activities.
 */
struct SceneEvent
{
    enum Type
    {
        Pop, Push
    } eventType;
    std::variant<std::monostate, std::shared_ptr<Scene>> nextScene;
};

class Scene  
{  
public:  
    std::queue<SceneEvent> events;

    void pushScene(std::shared_ptr<Scene> nextScene);
    void popScene();

    virtual ~Scene() = default;  

    // Update the scene - called in the main loop  
    virtual void step() = 0;  

    // Render the scene  
    virtual void render() = 0;  

    // The scene's event manager
    virtual void eventManager() = 0;

    // Handle user input  
    virtual void handleUserInput() = 0;  
};

class SceneManager
{  
private:
    sf::RenderWindow &window;
    std::stack<std::shared_ptr<Scene>> scenes;

    std::shared_ptr<Scene> & currentScene();

public:  
    SceneManager(sf::RenderWindow &window) : window(window) {}  

    void setScene(std::shared_ptr<Scene> scene);

    void update();
};