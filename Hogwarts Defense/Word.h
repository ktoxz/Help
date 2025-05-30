#pragma once
#include "Figure.h"
#include "std.h"

constexpr int BLACK = 97;
constexpr int GRAY = 98;
constexpr int WHITE = 99;
constexpr int RED = 100;
constexpr int PINK = 101;
constexpr int ARKBROWN = 102;
constexpr int BROWN = 103;
constexpr int ORANGE = 104;
constexpr int YELLOW = 105;
constexpr int DARKCYAN = 106;
constexpr int GREEN = 107;
constexpr int LIME = 108;
constexpr int DARKGUNMENTAL = 109;
constexpr int DARKBLUE = 110;
constexpr int BLUE = 111;
constexpr int CYAN = 112;


class Word : public Figure {
private:
	string word;
	short max_len;
	char color;
	void addCharacter(char new_char);
	void removeCharacter();
	void changeColor();
	void clear();
public:
	Word(short max_len = 10);
	Word(const string& src);

	~Word();

	void eraseAllCharacter();
	void setString(const string& src);
	string getWord();

	char getLastCharacter();
	void setCharacter(char src);

	void setColor(char color);
	void setMiddleCoordinate(short mid_coord_x, short coord_y);
};