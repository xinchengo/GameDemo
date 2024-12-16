#include "RenderedGameRunner.hpp"

RenderedGameRunner::RenderedGameRunner(sf::RenderWindow &window) : 
    GameRunner((float)window.getSize().x, (float)window.getSize().y),
    window(window) {}

void RenderedGameRunner::step()
{
    GameRunner::step();
    if(fish.getBoids().empty()) // If all the fish have been eaten
    {
        popScene();
        pushScene(winScene);
    }
    if(!eatenGreenCircles.empty()) // If the snake has eaten a green circle
    {
        popScene();
        pushScene(loseScene);
    }
}
void RenderedGameRunner::eventManager()
{
    if(snake)
    {
        snake->setVelocityFromMousePos(window);
    }
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
    // for(auto &fish : fishes)
    // {
    //     fish->render(window);
    // }
    fish.render(window);
    for(auto &circ : greenCircles)
    {
        circ->render(window);
    }
    if(snake)
    {
        snake->render(window);
    }
}

void RenderedGameRunner::bindWinScene(std::shared_ptr<Scene> scene)
{
    winScene = scene;
}

void RenderedGameRunner::bindLoseScene(std::shared_ptr<Scene> scene)
{
    loseScene = scene;
}
