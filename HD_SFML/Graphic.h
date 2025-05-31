#pragma once
#include <SFML/Graphics.hpp>

class Graphic {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    bool setPixel(const std::string& file_path);
    
    sf::Sprite& getSprite();



};
