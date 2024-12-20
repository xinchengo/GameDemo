#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <queue>
#include <memory>
#include <variant>
#include <optional>

#include "utilities/properties.hpp"

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

class Scene : virtual public HasTimer
{  
public:  
    std::queue<SceneEvent> events;

    void pushScene(std::shared_ptr<Scene> nextScene);
    void popScene();

    virtual ~Scene() = default;  

    // Called when the scene is activated
    virtual void onActivate() = 0;

    // Update the scene - called every frame 
    virtual void step() = 0;  

    // Render the scene - called every frame 
    virtual void render() = 0;  

    // The scene's event manager
    virtual void eventManager() = 0;
};

class SceneManager
{  
private:
    sf::RenderWindow &window;
    std::stack<std::shared_ptr<Scene>> scenes;
    std::queue<SceneEvent> events;

    std::shared_ptr<Scene> & currentScene();
    void fetchEventsFromScene();

public:  
    SceneManager(sf::RenderWindow &window) : window(window) {}  

    void setScene(std::shared_ptr<Scene> scene);

    void update();
};