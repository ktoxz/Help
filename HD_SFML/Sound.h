#ifndef SOUND_H
#define SOUND_H

#include <SFML/Audio.hpp>
#include <string>

class Sound {
public:
    Sound();
    Sound(const std::string& path);
    void play();
    void stop();
    void setLoop(bool l);
    void playNew(const std::string& path);

private:
    sf::Music music;
    bool loop = false;
};

#endif
