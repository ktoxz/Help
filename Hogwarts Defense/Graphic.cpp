#pragma once
#include "Graphic.h"

Graphic::Graphic() {
	n_row = n_col = 0;
	pixel = NULL;
};

Graphic::~Graphic() {
	if (pixel) {
		for (int i = 0; i < n_row; i++) {
			if (pixel[i]) delete[] pixel[i];
			pixel[i] = nullptr;
		}
		delete[] pixel;
	}
	pixel = nullptr;
}

void Graphic::setPixel(string file_name) {
	ifstream fi(file_name);
	fi >> n_row >> n_col;
	pixel = new char* [n_row];
	for (int i = 0; i < n_row; i++) {
		fi.get();
		pixel[i] = new char[n_col];
		for (int j = 0; j < n_col; j++)
			fi >> pixel[i][j];
	}

	fi.close();
}

char Graphic::getPixel(short x, short y) {
	if (x < 0 || x >= n_row || y < 0 || y >= n_col)
		return 0;
	return pixel[x][y];
}

void Graphic::setNumberOfRow(short N_ROW) {
	n_row = N_ROW;
}
short Graphic::getNumberOfRow() const {
	return n_row;
}
void Graphic::setNumberOfCol(short N_COL) {
	n_col = N_COL;
}
short Graphic::getNumberOfCol() const {
	return n_col;
}