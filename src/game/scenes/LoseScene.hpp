#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "game/SceneManager.hpp"
#include "utilities/AssetManager.hpp"

class LoseScene : public Scene
{
private:
    sf::RenderWindow &window;
    sf::Sprite text;

    void handleResize(sf::Vector2u size);
public:
    LoseScene(sf::RenderWindow &window);
    void onActivate() override;
    void render() override;
    void step() override;
    void eventManager() override;
};