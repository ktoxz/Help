#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Enemy {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<sf::Vector2f> path;
    size_t currentPoint = 0;
    float speed = 100.f; // pixels per second

public:
    Enemy(const std::string& texturePath, const std::vector<sf::Vector2f>& pathPoints);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Sprite& getSprite();
};
