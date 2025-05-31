#include "Graphic.h"

bool Graphic::setPixel(const std::string& file_path) {
    if (!texture.loadFromFile(file_path))
        return false;

    sprite.setTexture(texture);
    return true;
}

sf::Sprite& Graphic::getSprite() {
    return sprite;
}

