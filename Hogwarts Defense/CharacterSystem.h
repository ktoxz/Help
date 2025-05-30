#pragma once
#include "std.h"
#include "Graphic.h"

class CharacterSystem {
private:
	vector <Graphic*> list_character;
	CharacterSystem() {};
	CharacterSystem(const CharacterSystem&) = delete;
	CharacterSystem& operator=(const CharacterSystem&) = delete;
public:
	~CharacterSystem();
	static CharacterSystem& getInstance(string file_character = "");
	Graphic* getCharacter(char src);
};