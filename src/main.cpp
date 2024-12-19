#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include<cmath>
#include<nlohmann/json.hpp>

#include "game/SceneManager.hpp"
#include "game/scenes/RenderedGameRunner.hpp"
#include "game/scenes/StartScene.hpp"
#include "game/scenes/WinScene.hpp"
#include "game/scenes/LoseScene.hpp"
#include "utilities/config.hpp"
#include "utilities/AssetManager.hpp"

std::shared_ptr<RenderedGameRunner> newGame(sf::RenderWindow &window)
{
    auto game = std::make_shared<RenderedGameRunner>(window);
    game->bindLoseScene(std::make_shared<LoseScene>(window));
    game->bindWinScene(std::make_shared<WinScene>(window));
    game->newFish(100);
    game->newGreenCircles(3);
    game->newSnake(sf::Vector2f(0.0, 0.0), 20);
    return game;
}

int main()
{
    config.loadConfig("./assets/config.json");
    assetManager.loadAssets("./assets/assets.json");

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    // window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(config.gameFramerateLimit);

    bool paused = false;

    SceneManager sceneManager(window);

    auto start = std::make_shared<StartScene>(window);
    start->bindNewGameFunction(newGame);

    // sceneManager.setScene(std::static_pointer_cast<Scene, RenderedGameRunner>(game));
    sceneManager.setScene(std::static_pointer_cast<Scene, StartScene>(start));

    while (window.isOpen())
    {
        window.clear();
        sceneManager.update();
        window.display();
    }

    return 0;
}