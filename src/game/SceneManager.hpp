#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <queue>
#include <memory>
#include <variant>

class Scene;

struct SceneEvent
{
    enum Type
    {
        Pop, Push
    } eventType;
    std::variant<std::monostate, std::shared_ptr<Scene>> nextScene;
};

class Scene  
{  
public:  
    std::queue<SceneEvent> events;

    void pushScene(std::shared_ptr<Scene> nextScene);
    void popScene();

    virtual ~Scene() = default;  

    // Update the scene - called in the main loop  
    virtual void step() = 0;  

    // Render the scene  
    virtual void render() = 0;  

    // Handle user input  
    virtual void handleUserInput() = 0;  
};

class SceneManager
{  
private:
    sf::RenderWindow &window;  
    std::stack<std::shared_ptr<Scene>> scenes;

    std::shared_ptr<Scene> & currentScene();

public:  
    SceneManager(sf::RenderWindow &window) : window(window) {}  

    void setScene(std::shared_ptr<Scene> scene);

    void update();
};

/*class StartScene : public Scene {  
private:  
    sf::RenderWindow &window;
    sf::Font font;  
    sf::Text title;  
    sf::Text instruction;  

public:  
    StartScene(sf::RenderWindow &window) : window(window)
    {  
        // Load font  
        if (!font.loadFromFile("path/to/font.ttf")) { // Replace with the path to your font file  
            std::cerr << "Error loading font" << std::endl;  
        }  

        // Setup title text  
        title.setFont(font);  
        title.setString("Welcome to My Game");  
        title.setCharacterSize(50);  
        title.setFillColor(sf::Color::White);  
        title.setPosition(200, 100); // Position the title text  

        // Setup instruction text  
        instruction.setFont(font);  
        instruction.setString("Press Enter to Start");  
        instruction.setCharacterSize(30);  
        instruction.setFillColor(sf::Color::White);  
        instruction.setPosition(250, 200); // Position the instruction text  
    }  

    void step() override {  
        // Update logic if necessary  
    }  

    void render() override {  
        window.clear();  
        window.draw(title);  
        window.draw(instruction);  
        window.display();  
    }  

    void handleUserInput() override {  
        sf::Event event;  
        while (window.pollEvent(event)) {  
            if (event.type == sf::Event::Closed) {  
                window.close();  
            }  
            if (event.type == sf::Event::KeyPressed) {  
                if (event.key.code == sf::Keyboard::Enter) {  
                    // Trigger scene transition to the main game scene  
                    // You would typically put this logic in the SceneManager  
                    // For demonstration, we'll output to console  
                    std::cout << "Starting the game..." << std::endl;  
                    // Set up code to switch to the game scene would go here
            
                }  
            }  
        }  
    }  
};  */