#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button();
    Button(const std::string& imagePath);
    void setPosition(float x, float y);
    bool isClicked(sf::Vector2f mousePos) const;
    sf::Sprite& getSprite();
    sf::FloatRect getGlobalBounds() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif
