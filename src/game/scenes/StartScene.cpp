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

void StartScene::eventManager()
{
    sf::Event event;
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    while(window.pollEvent(event));
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if(startButton.getGlobalBounds().contains(worldPos))
            {
                pushScene(gameScene);
            }
            break;
        default:
            break;    
        }
    }
}

void StartScene::step()
{
}

void StartScene::handleUserInput()
{
    
}
