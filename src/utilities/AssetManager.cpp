#include "AssetManager.hpp"

bool TextureManager::loadFromFile(const std::string &name, const std::string &filePath)
{
    sf::Texture texture;  
    if(texture.loadFromFile(filePath))
    {  
        textures[name] = std::move(texture);  // Store in map
        return true;  
    }
    else
        return false; 
}

bool TextureManager::loadFromImage(const std::string &name, const sf::Image &image, sf::IntRect area)
{
    sf::Texture texture;  
    if(texture.loadFromImage(image, area))
    {  
        textures[name] = std::move(texture);  // Store in map
        return true;  
    }
    else
        return false; 
}

sf::Texture &TextureManager::get(const std::string &name)
{
    return textures.at(name);
}

bool FontManager::loadFromFile(const std::string &name, const std::string &filePath)
{
    sf::Font font;  
    if(font.loadFromFile(filePath))
    {  
        fonts[name] = std::move(font);  // Store in map
        return true;  
    }
    else
        return false; 
}

sf::Font &FontManager::get(const std::string &name)
{
    return fonts.at(name);
}

AssetManager &AssetManager::getInstance()
{
    static AssetManager instance; // Guaranteed to be destroyed.  
    return instance;                 // Instantiated on first use. 
}
