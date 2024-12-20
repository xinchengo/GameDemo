#include "StartScene.hpp"

#include "utilities/WinUtils.hpp"

StartScene::StartScene(sf::RenderWindow &window) : window(window)
{
    title.setTexture(assetManager.texture.get("gameTitle"), true);
    startButton.setTexture(assetManager.texture.get("startGame"), true);
    settingsButton.setTexture(assetManager.texture.get("gameSettings"), true);
    exitButton.setTexture(assetManager.texture.get("exitGame"), true);
    aboutButton.setTexture(assetManager.texture.get("aboutPage"), true);

    handleResize(window.getSize());
}

void StartScene::handleResize(sf::Vector2u windowSize)
{
    window.setView(getView(windowSize, config.gameScale));
    auto size = window.getView().getSize();

    title.setOrigin(title.getGlobalBounds().getSize() * 0.5f);
    title.setPosition(size.x * 0.5f, size.y * 0.2f);

    startButton.setOrigin(startButton.getGlobalBounds().getSize() * 0.5f);
    startButton.setPosition(size.x * 0.5f, size.y * 0.4f);

    settingsButton.setOrigin(settingsButton.getGlobalBounds().getSize() * 0.5f);
    settingsButton.setPosition(size.x * 0.5f, size.y * 0.55f);

    exitButton.setOrigin(exitButton.getGlobalBounds().getSize() * 0.5f);
    exitButton.setPosition(size.x * 0.5f, size.y * 0.70f);

    aboutButton.setOrigin(aboutButton.getGlobalBounds().getSize() * 0.5f);
    aboutButton.setPosition(size.x * 0.5f, size.y * 0.85f);
}

void StartScene::bindSettingsScene(std::shared_ptr<Scene> scene)
{
    settingsScene = scene;
}

void StartScene::bindAboutScene(std::shared_ptr<Scene> scene)
{
    aboutScene = scene;
}

void StartScene::onActivate()
{
    enableResize(window);
    handleResize(window.getSize());
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
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::Resized:
            handleResize(sf::Vector2u(event.size.width, event.size.height));
            break;
        case sf::Event::KeyPressed:
            if(event.key.code == config.gameToggleFullscreenKey)
            {
                toggleFullscreen(window);
                handleResize(window.getSize());
            }
            break;
        case sf::Event::MouseButtonPressed:
            if(startButton.getGlobalBounds().contains(worldPos))
            {
                pushScene(newGame(window));
            }
            else if(exitButton.getGlobalBounds().contains(worldPos))
            {
                window.close();
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

