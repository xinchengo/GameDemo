#pragma once

#include <SFML/Graphics.hpp>

#include "utilities/properties.hpp"
#include "game/SceneManager.hpp"
#include "game/GameRunner.hpp"

class RenderedGameRunner : public GameRunner, public Scene
{
private:
    sf::RenderWindow &window;
public:
    void step() override;
    
    void eventManager();
    void render();
    void handleUserInput();
    RenderedGameRunner(sf::RenderWindow &);
};
