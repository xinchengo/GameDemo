#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "game/SceneManager.hpp"
#include "utilities/AssetManager.hpp"

class WinScene : public Scene
{
private:
    sf::RenderWindow &window;
    sf::Sprite text;
public:
    WinScene(sf::RenderWindow &window) : window(window)
    {
        auto& assetManager = AssetManager::getInstance();
        text.setTexture(assetManager.texture.get("youWin"), true);
        text.setOrigin(text.getGlobalBounds().getSize() * 0.5f);
        text.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.5f);
    }
    void render() override;
    void step() override;
    void eventManager() override;
};