#pragma once
#include "std.h"

class Sound {
private:
	vector <wstring> list_sound;
	short n_list_sound, cur_sound;
	wstring id;
	short volume;
	bool flag_repeat, flag_mute;
	static wstring getID();
public:
	Sound(string file_sound);
	void set(short index);
	void play();
	void pause();
	void close();
	void resume();
	void increaseVolume();
	void decreaseVolume();
	void setVolume();
	short getVolume() const;
	void setFlagRepeat(bool flag);
	void setFlagMute(bool flag);
	bool getFlagMute() const;
};