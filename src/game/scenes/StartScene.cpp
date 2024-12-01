#include "StartScene.hpp"

void StartScene::bindGameScene(std::shared_ptr<Scene> scene)
{
    gameScene = scene;
}

void StartScene::bindSettingsScene(std::shared_ptr<Scene> scene)
{
    settingsScene = scene;
}

void StartScene::bindAboutScene(std::shared_ptr<Scene> scene)
{
    aboutScene = scene;
}

void StartScene::render()
{
    window.draw(title);
    window.draw(startButton);
    window.draw(settingsButton);
    window.draw(exitButton);
    window.draw(aboutButton);
}

void StartScene::step()
{
}

void StartScene::handleUserInput()
{
    
}
