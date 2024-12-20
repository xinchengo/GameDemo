#pragma once

#include <SFML/Graphics.hpp>
#include "utilities/properties.hpp"
#include "game/SceneManager.hpp"
#include "game/GameRunner.hpp"
#include "game/scenes/LoseScene.hpp"
#include "game/scenes/WinScene.hpp"

class RenderedGameRunner : public GameRunner, public Scene
{
private:
    sf::RenderWindow &window;
    std::function<std::shared_ptr<Scene>(sf::RenderWindow &)> winScene;
    std::function<std::shared_ptr<Scene>(sf::RenderWindow &)> loseScene;

    bool isPaused = false;

    bool delayActive = false; // Add this line
    std::chrono::steady_clock::time_point delayStartTime; // Add this line
    std::chrono::duration<float, std::ratio<1>> delayDuration; // Add this line

public:
    // friend std::shared_ptr<RenderedGameRunner> newGame(sf::RenderWindow &window);
    RenderedGameRunner(sf::RenderWindow &window);

    void onActivate() override;
    void step() override;
    void eventManager() override;
    void render() override;
};
