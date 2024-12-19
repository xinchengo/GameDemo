#pragma once

#include <SFML/Graphics.hpp>
#include "utilities/properties.hpp"
#include "game/SceneManager.hpp"
#include "game/GameRunner.hpp"

class RenderedGameRunner : public GameRunner, public Scene
{
private:
    sf::RenderWindow &window;
    std::shared_ptr<Scene> winScene;
    std::shared_ptr<Scene> loseScene;

    bool isPaused = false;
    sf::Keyboard::Key pauseKey; // Configurable pause key

    bool delayActive = false; // Add this line
    std::chrono::steady_clock::time_point delayStartTime; // Add this line
    std::chrono::duration<float, std::ratio<1>> delayDuration; // Add this line

public:
    RenderedGameRunner(sf::RenderWindow &window);

    void step() override;
    void eventManager() override;
    void render() override;
    void bindWinScene(std::shared_ptr<Scene> scene);
    void bindLoseScene(std::shared_ptr<Scene> scene);
};
