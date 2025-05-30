#include "Sound.h"
#pragma comment (lib, "winmm.lib")
#include <tchar.h>	
#include "std.h"
wstring Sound::getID() {
    static short ID = -1;
    ID++;
    return to_wstring(ID);
}

Sound::Sound(string file_sound) {
    ifstream fi(file_sound);
    fi >> n_list_sound;
    list_sound.resize(n_list_sound);
    for (auto& x : list_sound) {
        string tmp;
        fi >> tmp;
        wstring t(tmp.begin(), tmp.end());
        x = t;
    }
    cur_sound = 0;
    flag_repeat = 0;
    flag_mute = 0;
    id = Sound::getID();
    volume = 400;
    set(0);
    setVolume();
}

void Sound::set(short index) {
    if (index < 0 || index >= n_list_sound) return;
    cur_sound = index;
    close();
    wstring command = L"open " + list_sound[cur_sound] + L" type mpegvideo alias " + id;
    mciSendString(command.c_str(), NULL, 0, NULL);
    setVolume();
}

void Sound::play() {
    if (flag_mute == 1) return;
    //close();
    set(cur_sound);
    //setVolume();
    wstring command = L"play " + id;
    if (flag_repeat) command += L" repeat";
    mciSendString(command.c_str(), NULL, 0, NULL);
}

void Sound::pause() {
    wstring command = L"pause " + id;
    mciSendString(command.c_str(), NULL, 0, NULL);
}

void Sound::close() {
    wstring command = L"stop " + id;
    mciSendString(command.c_str(), NULL, 0, NULL);
    command = L"close " + id;
    mciSendString(command.c_str(), NULL, 0, NULL);
}

void Sound::resume() {
    if (flag_mute == 1) return;
    wstring command = L"resume " + id;
    mciSendString(command.c_str(), NULL, 0, NULL);
}

void Sound::increaseVolume() {
    if (volume < 1000) volume += 200;
    setVolume();
}

void Sound::decreaseVolume() {
    if (volume > 0) volume -= 200;
    setVolume();
}

void Sound::setVolume() {
    wstring command = L"setaudio " + id + L" volume to " + to_wstring(volume);
    mciSendString(command.c_str(), NULL, 0, NULL);
}

short Sound::getVolume() const {
    return volume;
}

void Sound::setFlagRepeat(bool flag) {
    flag_repeat = flag;
}

void Sound::setFlagMute(bool flag) {
    flag_mute = flag;
    if (flag_mute == true) pause();
    else resume();
}

bool Sound::getFlagMute() const {
    return flag_mute;
}


