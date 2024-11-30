#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<random>
#include<cmath>

#include "game/SceneManager.hpp"
#include "game/scenes/RenderedGameRunner.hpp"
#include "game/scenes/StartScene.hpp"
#include "utilities/config.hpp"
#include "utilities/AssetManager.hpp"

void load_assets();
void load_assets()
{
    auto& assetManager = AssetManager::getInstance();

    sf::Image images;
    images.loadFromFile("./assets/images/images.png");

    assetManager.texture.loadFromImage("snakeBody", images, sf::IntRect(0, 0, 48, 48));
    assetManager.texture.loadFromImage("greenCircle", images, sf::IntRect(72, 0, 52, 52));
    assetManager.texture.loadFromImage("gameTitle", images, sf::IntRect(144, 0, 576, 96));
}

int main()
{
    load_assets();

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    window.setVerticalSyncEnabled(true);

    bool paused = false;

    SceneManager sceneManager(window);

    auto game = std::make_shared<RenderedGameRunner>(window);
    game->newRandomFish(CONST::FISH_STRATEGY::LINEAR, 30);
    game->newRandomFish(CONST::FISH_STRATEGY::BASELINE, 30);
    game->newGreenCircles(10);

    auto start = std::make_shared<StartScene>(window);
    start->bindGameScene(std::static_pointer_cast<Scene, RenderedGameRunner>(game));

    // sceneManager.setScene(std::static_pointer_cast<Scene, RenderedGameRunner>(game));
    sceneManager.setScene(std::static_pointer_cast<Scene, StartScene>(start));


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