#include "WinScene.hpp"

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
    while(window.pollEvent(event));
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
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
