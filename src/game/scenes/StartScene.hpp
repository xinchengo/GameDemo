#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "game/SceneManager.hpp"
#include "utilities/AssetManager.hpp"

class StartScene : public Scene
{
private:
    sf::RenderWindow &window;
    
    std::shared_ptr<Scene> gameScene;
    std::shared_ptr<Scene> settingsScene;
    std::shared_ptr<Scene> aboutScene;

    sf::Sprite title;
    
    
public:

    StartScene(sf::RenderWindow &window) : window(window)
    {
        auto& assetManager = AssetManager::getInstance();
        title.setTexture(assetManager.texture.get("gameTitle"), true);
        title.setOrigin(title.getGlobalBounds().getSize() * 0.5f);
        title.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.3f);
    }
    void bindGameScene(std::shared_ptr<Scene> scene);
    void bindSettingsScene(std::shared_ptr<Scene> scene);
    void bindAboutScene(std::shared_ptr<Scene> scene);

    void render() override;
    void step() override;
    void handleUserInput() override;
};