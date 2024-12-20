#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include<cmath>
#include<nlohmann/json.hpp>
#include<memory>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "game/SceneManager.hpp"
#include "game/scenes/StartScene.hpp"
#include "utilities/config.hpp"
#include "utilities/WinUtils.hpp"
#include "utilities/AssetManager.hpp"

int main(int argc, char *argv[])
{
    config.loadConfig("./assets/config.json");
    assetManager.loadAssets("./assets/assets.json");

    if(config.gameApplyScaleInJSON == false)
    {
        config.gameScale = getWindowScale();
    }

    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(config.gameWindowWidth * config.gameScale),
        static_cast<unsigned int>(config.gameWindowHeight * config.gameScale)),
        sf::String::fromUtf8(config.gameWindowTitle.begin(), config.gameWindowTitle.end()));

    window.setIcon(assetManager.texture.get("icon").getSize().x, assetManager.texture.get("icon").getSize().y, assetManager.texture.get("icon").copyToImage().getPixelsPtr());

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

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif