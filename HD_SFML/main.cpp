#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Graphic.h"
#include "Sound.h"
#include "Button.h"
#include "Enemy.h"

void scaleToWindow(sf::Sprite& sprite, const sf::RenderWindow& window) {
         sf::Vector2u textureSize = sprite.getTexture()->getSize();
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    sprite.setScale(scaleX, scaleY);
}

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML Fullscreen", sf::Style::Fullscreen);
    sf::Clock clock;

    // Load background images
    Graphic startBackground, homeBackground, chonMapBackground, bandoBackground, huffBackground;
    startBackground.setPixel("Start/BG.png");
    homeBackground.setPixel("Home/Background.png");
    chonMapBackground.setPixel("Start/Chonmap.png");
    bandoBackground.setPixel("Start/Bando.png");
    huffBackground.setPixel("Start/huff.png");

    sf::Sprite& sprite1 = startBackground.getSprite(); scaleToWindow(sprite1, window);
    sf::Sprite& sprite2 = homeBackground.getSprite(); scaleToWindow(sprite2, window);
    sf::Sprite& sprite3 = chonMapBackground.getSprite(); scaleToWindow(sprite3, window);
    sf::Sprite& sprite4 = bandoBackground.getSprite(); scaleToWindow(sprite4, window);
    sf::Sprite& sprite5 = huffBackground.getSprite(); scaleToWindow(sprite5, window);

    // Buttons
    Button playButton("Start/play.png");
    Button GoToMapButton("Home/GoToMap.png");
    Button quayVeButton("Start/quayve.png");

    sf::Vector2u winSize = window.getSize();
    sf::FloatRect playBounds = playButton.getGlobalBounds();
    playButton.setPosition(winSize.x / 2.f - playBounds.width / 2.f + 70.f, winSize.y * 3.f / 4.f - 400.f);
    sf::FloatRect GoToMapBounds = GoToMapButton.getGlobalBounds();
    GoToMapButton.setPosition(winSize.x / 2.f - GoToMapBounds.width / 2.f - 50.f, winSize.y * 3.f / 4.f + 50.f);

    sf::FloatRect quayveBounds = quayVeButton.getGlobalBounds();
    quayVeButton.setPosition(30, winSize.y - quayveBounds.height - 30);

    // Music
    Sound musicStart("Sound/start.wav");
    Sound musicHome("Sound/home.wav");
    Sound musicChonMap("Sound/LucChonMap.wav");
    Sound musicMap12("Sound/Map12.wav");

    musicStart.setLoop(true);
    musicStart.play();

    int currentScreen = 1;

    std::vector<sf::Vector2f> path1 = {
        {590, 930}, {590, 770}, {200, 770}, {200, 480}, {1100, 480},{1130, 420}
    };
    std::vector<sf::Vector2f> path2 = {
        {630, 980}, {630, 820}, {240, 820}, {240, 550}, {1110, 550},{1140, 470}
    };

    std::vector<sf::Vector2f> path3 = {
        {550, 980}, {550, 820}, {220, 820}, {220, 550}, {1100, 550},{1130, 470}
    };

    // Tính scale tương ứng ảnh bản đồ
    sf::Vector2f scaleFactor(
        static_cast<float>(winSize.x) / sprite5.getTexture()->getSize().x,
        static_cast<float>(winSize.y) / sprite5.getTexture()->getSize().y
    );

    for (auto& p2 : path2) {
        p2.x *= scaleFactor.x;
        p2.y *= scaleFactor.y;
    }
    for (auto& p1 : path1) {
        p1.x *= scaleFactor.x;
        p1.y *= scaleFactor.y;
    }
    for (auto& p3 : path3) {
        p3.x *= scaleFactor.x;
        p3.y *= scaleFactor.y;
    }

    // Tạo enemy
    Enemy enemy1("Enemy/enemy1.png", path1);
    Enemy enemy2("Enemy/enemy2.png", path2);
    Enemy enemy3("Enemy/enemy3.png", path3);

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (currentScreen == 1 && playButton.isClicked(mousePos)) {
                    musicStart.stop();
                    musicHome.setLoop(true);
                    musicHome.play();
                    currentScreen = 2;
                }
                else if (currentScreen == 2 && GoToMapButton.isClicked(mousePos)) {
                    musicHome.stop();
                    musicChonMap.setLoop(true);
                    musicChonMap.play();
                    currentScreen = 3;
                }
                else if (currentScreen == 3) {
                    if (quayVeButton.isClicked(mousePos)) {
                        musicChonMap.stop();
                        musicHome.setLoop(true);
                        musicHome.play();
                        currentScreen = 2;
                    }
                    else if (playButton.isClicked(mousePos)) {
                        currentScreen = 4;
                    }
                }
                else if (currentScreen == 4 && playButton.isClicked(mousePos)) {
                    musicChonMap.stop();
                    musicMap12.setLoop(true);
                    musicMap12.play();
                    currentScreen = 5;
                }
            }
        }

        if (currentScreen == 5) {
            enemy1.update(dt);
            enemy2.update(dt);
            enemy3.update(dt);
        }

        window.clear();

        if (currentScreen == 1) {
            window.draw(sprite1);
            window.draw(playButton.getSprite());
        }
        else if (currentScreen == 2) {
            window.draw(sprite2);
            window.draw(GoToMapButton.getSprite());
        }
        else if (currentScreen == 3) {
            window.draw(sprite3);
            window.draw(quayVeButton.getSprite());
            window.draw(playButton.getSprite());
        }
        else if (currentScreen == 4) {
            window.draw(sprite4);
            window.draw(playButton.getSprite());
        }
        else if (currentScreen == 5) {
            window.draw(sprite5);
            enemy1.draw(window);
            enemy2.draw(window);
            enemy3.draw(window);
        }

        window.display();
    }

    return 0;
}
