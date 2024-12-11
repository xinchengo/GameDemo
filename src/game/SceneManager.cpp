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
    if (!scenes.empty()) 
    {
        currentScene()->eventManager();

        while(currentScene()->events.empty() == false)
        {
            auto &event = currentScene()->events.front();
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
        }
        currentScene()->handleUserInput();  
        currentScene()->step();  
        currentScene()->render();  
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
