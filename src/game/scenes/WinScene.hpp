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

    void WinScene::handleResize(sf::Vector2u windowSize);
public:
    WinScene(sf::RenderWindow &window); // Declaration of constructor
    void onActivate() override;
    void render() override;
    void step() override;
    void eventManager() override;
};