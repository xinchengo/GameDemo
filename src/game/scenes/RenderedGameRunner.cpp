#include "RenderedGameRunner.hpp"
#include <SFML/Window/Keyboard.hpp>

#include "utilities/Winutils.hpp"

std::shared_ptr<RenderedGameRunner> newGame(sf::RenderWindow &window)
{
    auto game = std::make_shared<RenderedGameRunner>(window);
    game->newFish(100);
    game->newGreenCircles(3);
    game->newSnake(sf::Vector2f(0.0, 0.0), 20);
    return game;
}

RenderedGameRunner::RenderedGameRunner(sf::RenderWindow &window) : 
    GameRunner((float)window.getView().getSize().x, 
                (float)window.getView().getSize().y),
    window(window)
{
    delayDuration = std::chrono::duration<float, std::ratio<1>>(config.gameDelayWhenGameIsOver);
}

void RenderedGameRunner::onActivate()
{
    disableResize(window);
}

void RenderedGameRunner::step()
{
    if (delayActive) // Check if delay is active
    {
        auto currentTime = timer.now();
        if (currentTime - delayStartTime >= delayDuration)
        {
            delayActive = false;
            isPaused = false;
            if (fish.getBoids().empty()) // If all the fish have been eaten
            {
                popScene();
                pushScene(std::make_shared<WinScene>(window));
            }
            else if (!eatenGreenCircles.empty()) // If the snake has eaten a green circle
            {
                popScene();
                pushScene(std::make_shared<LoseScene>(window));
            }
        }
        return; // Skip updates during delay
    }
    
    if (isPaused) 
        return; // Skip updates when paused

    GameRunner::step();
    if (fish.getBoids().empty() || !eatenGreenCircles.empty()) // If all the fish have been eaten or the snake has eaten a green circle
    {
        isPaused = true; // Pause the game
        delayActive = true; // Activate delay
        delayStartTime = timer.now(); // Start the delay timer
    }
}

void RenderedGameRunner::eventManager()
{
    if(snake)
    {
        snake->setVelocityFromMousePos(window, frameDuration);
    }
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if(event.key.code == config.gamePauseKey)
            {
                isPaused = !isPaused; // Toggle pause state
            }
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
    for(auto &circ : greenCircles)
    {
        circ->render(window);
    }
    if(snake)
    {
        snake->render(window);
    }
    fish.render(window);
}