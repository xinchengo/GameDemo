#include "SceneManager.hpp"

std::shared_ptr<Scene> & SceneManager::currentScene()
{
    return scenes.top();
}

void SceneManager::setScene(std::shared_ptr<Scene> scene)
{
    scenes.emplace(std::move(scene));
}

void SceneManager::update()
{
    fetchEventsFromScene();
    while(events.empty() == false)
    {
        auto event = std::move(events.front());
        events.pop();

        switch (event.eventType)
        {
        case SceneEvent::Type::Pop:
            scenes.pop();
            break;
        case SceneEvent::Type::Push:
            scenes.emplace(std::get<std::shared_ptr<Scene>>(event.nextScene));
            break;
        default:
            break;
        }
        
        fetchEventsFromScene();
    }
    if (!scenes.empty()) 
    {
        
        currentScene()->eventManager();
        currentScene()->step();  
        currentScene()->render();  
    }
}

void SceneManager::fetchEventsFromScene()
{
    if (scenes.empty() == false)
    {
        auto &scene = currentScene();
        while (!scene->events.empty())
        {
            events.push(scene->events.front());
            scene->events.pop();
        }
    }
}

void Scene::pushScene(std::shared_ptr<Scene> nextScene)
{
    SceneEvent event;
    event.eventType = SceneEvent::Type::Push;
    event.nextScene = nextScene;
    events.emplace(event);
}

void Scene::popScene()
{
    SceneEvent event;
    event.eventType = SceneEvent::Type::Pop;
    events.emplace(event);
}
