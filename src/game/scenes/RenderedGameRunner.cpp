#include "RenderedGameRunner.hpp"
#include <SFML/Window/Keyboard.hpp>

RenderedGameRunner::RenderedGameRunner(sf::RenderWindow &window) : 
    GameRunner((float)window.getSize().x, (float)window.getSize().y),
    window(window)
{
    pauseKey = static_cast<sf::Keyboard::Key>(config.gamePauseKey);
    delayDuration = std::chrono::duration<float, std::ratio<1>>(config.gameDelayWhenGameIsOver);
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
                pushScene(winScene);
            }
            else if (!eatenGreenCircles.empty()) // If the snake has eaten a green circle
            {
                popScene();
                pushScene(loseScene);
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
            if(event.key.code == pauseKey)
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

void RenderedGameRunner::bindWinScene(std::shared_ptr<Scene> scene)
{
    winScene = scene;
}

void RenderedGameRunner::bindLoseScene(std::shared_ptr<Scene> scene)
{
    loseScene = scene;
}