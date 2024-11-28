#include "RenderedGameRunner.hpp"

RenderedGameRunner::RenderedGameRunner(sf::RenderWindow &window) : 
    GameRunner((float)window.getSize().x, (float)window.getSize().y),
    window(window) {}

void RenderedGameRunner::handleUserInput()
{
    snake.setVelocityFromMousePos(window);
}
void RenderedGameRunner::render()
{
    for(auto &eater : eaters)
    {
        eater->render(window);
    }
    for(auto &fish : fishes)
    {
        if(!fish->isDead(frameNumber))
        {
            fish->render(window);
        }
    }
    for(auto &circ : greenCircles)
    {
        circ->render(window);
    }
    snake.render(window);
}