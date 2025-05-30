#pragma once
#include "std.h"

class Graphic {
private:
	char** pixel;
	int n_row, n_col;

public:
	Graphic();
	~Graphic();
	void setPixel(string);
	char getPixel(short, short);
	void setNumberOfRow(short);
	short getNumberOfRow() const;
	void setNumberOfCol(short);
	short getNumberOfCol() const;

	friend class Word;
};