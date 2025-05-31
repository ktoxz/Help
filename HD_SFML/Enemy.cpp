#include "Enemy.h"
#include <cmath>

Enemy::Enemy(const std::string& texturePath, const std::vector<sf::Vector2f>& pathPoints)
    : path(pathPoints)
{
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture);
    if (!path.empty()) {
        sprite.setPosition(path[0].x, path[0].y);
    }
}

void Enemy::update(float deltaTime) {
    if (currentPoint + 1 >= path.size()) return;

    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f target = path[currentPoint + 1];
    sf::Vector2f direction = target - currentPos;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0.f)
        direction /= length;

    float distanceToMove = speed * deltaTime;
    if (length <= distanceToMove) {
        sprite.setPosition(target);
        currentPoint++;
    }
    else {
        sprite.move(direction * distanceToMove);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Sprite& Enemy::getSprite() {
    return sprite;
}
