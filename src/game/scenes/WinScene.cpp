#include "WinScene.hpp"

#include "utilities/WinUtils.hpp"

void WinScene::handleResize(sf::Vector2u windowSize)
{
    sf::FloatRect visibleArea(0, 0, 
        static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    window.setView(sf::View(visibleArea));
    auto size = window.getView().getSize();

    text.setOrigin(text.getGlobalBounds().getSize() * 0.5f);
    text.setPosition(size.x * 0.5f, size.y * 0.5f);
}

WinScene::WinScene(sf::RenderWindow& window) : window(window)
{
    text.setTexture(assetManager.texture.get("youWin"), true);
    handleResize(window.getSize());
}

void WinScene::onActivate()
{
    enableResize(window);
    handleResize(window.getSize());
}

void WinScene::render()
{
    window.draw(text);
}

void WinScene::step()
{
}

void WinScene::eventManager()
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
            if(event.key.code == sf::Keyboard::Key::F)
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
