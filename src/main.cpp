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
#include "utilities/config.hpp"
#include "utilities/AssetManager.hpp"

void load_assets();
void load_assets()
{
    auto& assetManager = AssetManager::getInstance();

    std::ifstream assetsList("./assets/assets.json");
    auto data = nlohmann::json::parse(assetsList);

    // import textures
    for(auto &image : data["textures"].items())
    {
        std::string imagePath = image.key();
        auto& imageData = image.value();

        sf::Image imageFile;
        imageFile.loadFromFile("./assets/" + imagePath);

        for(auto &texture : imageData.items()) {
            std::string textureName = texture.key();
            auto &textureDetails = texture.value();

            int x = textureDetails["x"];
            int y = textureDetails["y"];
            int width = textureDetails["width"];
            int height = textureDetails["height"];

            assetManager.texture.loadFromImage(textureName, imageFile, sf::IntRect(x, y, width, height));
        }
    }
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
        window.clear();
        sceneManager.update();
        window.display();
    }

    return 0;
}