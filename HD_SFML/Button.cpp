#include "Button.h"
#include <iostream>

Button::Button() {
}

Button::Button(const std::string& imagePath) {
    texture.loadFromFile(imagePath);
    sprite.setTexture(texture);
}

void Button::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

bool Button::isClicked(sf::Vector2f mousePos) const {
    return sprite.getGlobalBounds().contains(mousePos);
}

sf::Sprite& Button::getSprite() {
    return sprite;
}

sf::FloatRect Button::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
