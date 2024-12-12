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

public:
    void step() override;
    
    void eventManager();
    void render();
    void bindWinScene(std::shared_ptr<Scene> scene);
    void bindLoseScene(std::shared_ptr<Scene> scene);
    RenderedGameRunner(sf::RenderWindow &);
};
