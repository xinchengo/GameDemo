#include "LoseScene.hpp"

#include "utilities/WinUtils.hpp"

void LoseScene::handleResize(sf::Vector2u windowSize)
{
    window.setView(getView(windowSize, config.gameScale));
    auto size = window.getView().getSize();

    text.setOrigin(text.getGlobalBounds().getSize() * 0.5f);
    text.setPosition(size.x * 0.5f, size.y * 0.5f);
}

LoseScene::LoseScene(sf::RenderWindow &window) : window(window)
{
    text.setTexture(assetManager.texture.get("youLose"), true);
    handleResize(window.getSize());
}

void LoseScene::render()
{
    window.draw(text);
}

void LoseScene::onActivate()
{
    enableResize(window);
    handleResize(window.getSize());
}

void LoseScene::step()
{
}

void LoseScene::eventManager()
{
    sf::Event event;
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::Resized:
            handleResize(sf::Vector2u(event.size.width, event.size.height));
            break;
        case sf::Event::KeyPressed:
            if(event.key.code == config.gameToggleFullscreenKey)
            {
                toggleFullscreen(window);
                handleResize(window.getSize());
            }
            break;
        case sf::Event::MouseButtonPressed:
            if(text.getGlobalBounds().contains(worldPos))
            {
                popScene();
            }
            break;
        default:
            break;    
        }
    }
}
