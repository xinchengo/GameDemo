#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include<cmath>
#include<nlohmann/json.hpp>

#include "game/SceneManager.hpp"
#include "game/scenes/StartScene.hpp"
#include "utilities/config.hpp"
#include "utilities/AssetManager.hpp"

int main()
{
    config.loadConfig("./assets/config.json");
    assetManager.loadAssets("./assets/assets.json");

    sf::RenderWindow window(sf::VideoMode(config.gameWindowWidth * config.gameScale,
         config.gameWindowHeight * config.gameScale), 
        sf::String::fromUtf8(config.gameWindowTitle.begin(), config.gameWindowTitle.end()));

    // window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(config.gameFramerateLimit);

    bool paused = false;

    SceneManager sceneManager(window);

    auto start = std::make_shared<StartScene>(window);

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