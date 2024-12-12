#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include "game/SceneManager.hpp"
#include "game/scenes/RenderedGameRunner.hpp"
#include "utilities/AssetManager.hpp"

class StartScene : public Scene
{
private:
    sf::RenderWindow &window;
    
    std::shared_ptr<Scene> settingsScene;
    std::shared_ptr<Scene> aboutScene;

    sf::Sprite title;
    sf::Sprite startButton;
    sf::Sprite settingsButton;
    sf::Sprite exitButton;
    sf::Sprite aboutButton;

    std::function<std::shared_ptr<Scene>(sf::RenderWindow &)> newGame;
    
public:

    StartScene(sf::RenderWindow &window) : window(window)
    {
        auto& assetManager = AssetManager::getInstance();
        title.setTexture(assetManager.texture.get("gameTitle"), true);
        title.setOrigin(title.getGlobalBounds().getSize() * 0.5f);
        title.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.2f);

        startButton.setTexture(assetManager.texture.get("startGame"), true);
        startButton.setOrigin(startButton.getGlobalBounds().getSize() * 0.5f);
        startButton.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.4f);

        settingsButton.setTexture(assetManager.texture.get("gameSettings"), true);
        settingsButton.setOrigin(settingsButton.getGlobalBounds().getSize() * 0.5f);
        settingsButton.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.55f);

        exitButton.setTexture(assetManager.texture.get("exitGame"), true);
        exitButton.setOrigin(exitButton.getGlobalBounds().getSize() * 0.5f);
        exitButton.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.70f);

        aboutButton.setTexture(assetManager.texture.get("aboutPage"), true);
        aboutButton.setOrigin(aboutButton.getGlobalBounds().getSize() * 0.5f);
        aboutButton.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.85f);
    }
    void bindNewGameFunction(std::function<std::shared_ptr<Scene>(sf::RenderWindow &)> func);
    void bindSettingsScene(std::shared_ptr<Scene> scene);
    void bindAboutScene(std::shared_ptr<Scene> scene);

    void render() override;
    void step() override;
    void eventManager() override;
};