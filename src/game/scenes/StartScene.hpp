#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include "game/SceneManager.hpp"
#include "game/scenes/RenderedGameRunner.hpp"
#include "utilities/AssetManager.hpp"

std::shared_ptr<RenderedGameRunner> newGame(sf::RenderWindow &window);

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
    
    void handleResize(sf::Vector2u windowSize);
    
public:

    StartScene(sf::RenderWindow &window);
    void bindSettingsScene(std::shared_ptr<Scene> scene);
    void bindAboutScene(std::shared_ptr<Scene> scene);

    void onActivate() override;
    void render() override;
    void step() override;
    void eventManager() override;
};