#include "Sound.h"
#include <iostream>

Sound::Sound() {
    // Constructor mặc định
}

Sound::Sound(const std::string& path) {
    if (!music.openFromFile(path)) {
        std::cerr << "Không thể mở nhạc: " << path << "\n";
    }
}

void Sound::play() {
    music.play();
}

void Sound::stop() {
    music.stop();
}

void Sound::setLoop(bool l) {
    loop = l;
    music.setLoop(l);
}

void Sound::playNew(const std::string& path) {
    if (!music.openFromFile(path)) {
        std::cerr << "Không thể mở nhạc: " << path << "\n";
        return;
    }
    music.setLoop(loop);
    music.play();
}
