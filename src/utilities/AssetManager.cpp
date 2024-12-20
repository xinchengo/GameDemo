#include "AssetManager.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

AssetManager assetManager;

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
        texture.setSmooth(true);
        texture.generateMipmap();
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

void AssetManager::loadAssets(std::string fileName)
{
    std::ifstream assetsList(fileName);
    auto data = nlohmann::json::parse(assetsList);

    // import textures
    for(auto &image : data["textures"].items())
    {
        std::string imagePath = image.key();
        auto& imageData = image.value();

        sf::Image imageFile;
        imageFile.loadFromFile("./assets/" + imagePath);

        for(auto &textureTerm : imageData.items()) {
            std::string textureName = textureTerm.key();
            auto &textureDetails = textureTerm.value();

            int x = textureDetails["x"];
            int y = textureDetails["y"];
            int width = textureDetails["width"];
            int height = textureDetails["height"];

            texture.loadFromImage(textureName, imageFile, sf::IntRect(x, y, width, height));
        }
    }
}