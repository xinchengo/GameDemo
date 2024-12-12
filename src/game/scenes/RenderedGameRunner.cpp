#include "RenderedGameRunner.hpp"

RenderedGameRunner::RenderedGameRunner(sf::RenderWindow &window) : 
    GameRunner((float)window.getSize().x, (float)window.getSize().y),
    window(window) {}

void RenderedGameRunner::handleUserInput()
{
    snake.setVelocityFromMousePos(window);
}
void RenderedGameRunner::step()
{
    GameRunner::step();
    if(fishes.empty()) // If all the fish have been eaten
    {
        popScene();
    }
    if(!eatenGreenCircles.empty()) // If the snake has eaten a green circle
    {
        popScene();
    }
}
void RenderedGameRunner::eventManager()
{
    sf::Event event;
    while(window.pollEvent(event));
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;    
        }
    }
}
void RenderedGameRunner::render()
{
    for(auto &fish : fishes)
    {
        fish->render(window);
    }
    for(auto &circ : greenCircles)
    {
        circ->render(window);
    }
    snake.render(window);
}