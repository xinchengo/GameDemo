#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<random>
#include<cmath>

#include "game/SceneManager.hpp"
#include "game/scenes/RenderedGameRunner.hpp"
#include "utilities/config.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    window.setVerticalSyncEnabled(true);

    bool paused = false;

    SceneManager sceneManager(window);

    auto game = std::make_shared<RenderedGameRunner>(window);
    game->newRandomFish(CONST::FISH_STRATEGY::LINEAR, 30);
    game->newRandomFish(CONST::FISH_STRATEGY::BASELINE, 30);
    game->newGreenCircles(10);

    sceneManager.setScene(std::static_pointer_cast<Scene, RenderedGameRunner>(game));


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        window.clear();
        sceneManager.update();
        window.display();
    }

    return 0;
}