#include "CharacterSystem.h"

CharacterSystem::~CharacterSystem() {
	for (auto& x : list_character) {
		if (x) delete x;
		x = nullptr;
	}
	list_character.clear();
}

CharacterSystem& CharacterSystem::getInstance(string file_character)
{
	static CharacterSystem obj;
	static bool flag(false);
	if (flag == false) {
		obj.list_character.resize(128, nullptr);
		ifstream fi(file_character);
		if (fi) {
			short n;
			fi >> n;
			fi.get();
			string tmp2;
			char tmp1;
			for (int i = 0; i < n; i++) {
				tmp1 = fi.get();
				fi >> tmp2; fi.get();
				//fi >> tmp1 >> tmp2;
				if (tmp1 < 0 || tmp1 >= 128) continue;
				obj.list_character[tmp1] = new Graphic;
				obj.list_character[tmp1]->setPixel(tmp2);
			}
		}
		fi.close();
		flag = true;
	}
	return ref(obj);
}

Graphic* CharacterSystem::getCharacter(char src)
{
	if (src < 0) return nullptr;
	return list_character[src];
}
