#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<random>
#include<cmath>

#include "entities.hpp"
#include "gameRunner.hpp"
#include "utils.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    window.setVerticalSyncEnabled(true);

    bool paused = false;

    RenderedGameRunner game(window);
    game.newRandomFish(CONST::FISH_STRATEGY::LINEAR, 30);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        window.clear();
        game.step();
        game.render();
        window.display();
    }

    return 0;
}