#pragma once
#include "Figure.h"
#include "utility.h"
#include "std.h"

Figure::Figure() {
	n_costume = 0;
	costume = nullptr;
	cur_costume = 0;
	state = 0;
	scale_factor = 1;
	direction = 1;
}

Figure::Figure(short src_x, short src_y, string file_costume, bool src_state) : Figure() {
	setCurrentCoordinate(src_x, src_y);
	setCostume(file_costume);
	state = src_state;
}

Figure::~Figure() {
	if (costume) delete[] costume;
	costume = nullptr;
}

Graphic& Figure::getCostumeAtIndex(short index) {
	if (index < 0 || index >= n_costume) exit(1);
	return costume[index];
}

Graphic& Figure::getCurrentCostume() {
	return costume[cur_costume];
}

void Figure::setCostume(string file_name) {
	ifstream fi(file_name);
	if (!fi) return;
	short tmp;
	fi >> tmp;
	fi.get();
	setNumberOfCostume(tmp);
	costume = new Graphic[n_costume];
	for (int i = 0; i < n_costume; i++) {
		string file_costume;
		getline(fi, file_costume);
		costume[i].setPixel(file_costume);
	}
	fi.close();
}

void Figure::setNumberOfCostume(short src) {
	if (src < 0) exit(1000);
	n_costume = src;
}

short Figure::getNumberOfCostume() const {
	return n_costume;
}

void Figure::setIndexCurrentCostume(short src) {
	if (src >= 0 && src < n_costume) cur_costume = src;
}

short Figure::getIndexCurrentCostume() {
	return cur_costume;
}

Coordinate& Figure::getCurrentCoordinate() {
	return cur_coord;
}

Coordinate Figure::getCurrentCenterCoordinate()
{
	Coordinate tmp(cur_coord.getCoordX() + costume[cur_costume].getNumberOfCol() / 2
		, cur_coord.getCoordY() + costume[cur_costume].getNumberOfRow() / 2);
	return tmp;
}

void Figure::setCurrentCoordinate(short x, short y) {
	cur_coord.setCoordX(x);
	cur_coord.setCoordY(y);
}

void Figure::setCurrentCoordinate(const Coordinate& src) {
	cur_coord = src;
}

void Figure::setState(bool src) {
	state.store(src);
}

bool Figure::getState() const {
	return state.load();
}

void Figure::setFlagStop(bool src) {
	flag_stop = src;
}

bool Figure::getFlagStop() const {
	return flag_stop;
}

void Figure::setFlagPause(bool src) {
	flag_pause = src;
}

bool Figure::getFlagPause() const {
	return flag_pause;
}

void Figure::setScaleFactor(double src) {
	if (src > 0) scale_factor = src;
}

double Figure::getScaleFactor() const {
	return scale_factor;
}

void Figure::setDirection(bool src) {
	direction.store(src);
}

bool Figure::getDirection() {
	return direction.load();
}

bool Figure::checkCollision(Figure* b)
{
	//return true;
	// Store coordinate of 4 corners of bullet
	pair<short, short> a1(this->getCurrentCoordinate().getCoordX(), this->getCurrentCoordinate().getCoordY());
	pair<short, short> a2(a1.first + getCurrentCostume().getNumberOfCol() - 1, a1.second);
	pair<short, short> a3(a1.first + getCurrentCostume().getNumberOfCol() - 1, a1.second + getCurrentCostume().getNumberOfRow() - 1);
	pair<short, short> a4(a1.first, a1.second + getCurrentCostume().getNumberOfRow() - 1);

	// Store coordinate of 4 corners of Figure b
	pair<short, short> b1(b->getCurrentCoordinate().getCoordX(), b->getCurrentCoordinate().getCoordY());
	pair<short, short> b2(b1.first + getCurrentCostume().getNumberOfCol() - 1, b1.second);
	pair<short, short> b3(b1.first + getCurrentCostume().getNumberOfCol() - 1, b1.second + getCurrentCostume().getNumberOfRow() - 1);
	pair<short, short> b4(b1.first, b1.second + getCurrentCostume().getNumberOfRow() - 1);

	//Check if there is any corner is in other Figure area 
	bool check1 = (b1.first >= a1.first && b1.first <= a3.first) && (b1.second >= a1.second && b1.second <= a3.second);
	bool check2 = (b2.first >= a1.first && b2.first <= a3.first) && (b2.second >= a1.second && b2.second <= a3.second);
	bool check3 = (b3.first >= a1.first && b3.first <= a3.first) && (b3.second >= a1.second && b3.second <= a3.second);
	bool check4 = (b4.first >= a1.first && b4.first <= a3.first) && (b4.second >= a1.second && b4.second <= a3.second);

	bool check5 = (a1.first >= b1.first && a1.first <= b3.first) && (a1.second >= b1.second && a1.second <= b3.second);
	bool check6 = (a2.first >= b1.first && a2.first <= b3.first) && (a2.second >= b1.second && a2.second <= b3.second);
	bool check7 = (a3.first >= b1.first && a3.first <= b3.first) && (a3.second >= b1.second && a3.second <= b3.second);
	bool check8 = (a4.first >= b1.first && a4.first <= b3.first) && (a4.second >= b1.second && a4.second <= b3.second);

	if (check1 || check2 || check3 || check4 || check5 || check6 || check7 || check8) {
		int left = (a1.first > b1.first) ? a1.first : b1.first;
		int right = (a2.first < b2.first) ? a2.first : b2.first;

		int top = (a1.second > b1.second) ? a1.second : b1.second;
		int bottom = (a3.second < b3.second) ? a3.second : b3.second;

		for (int i = top; i <= bottom; i++) {
			for (int j = left; j <= right; j++) {
				if (getCurrentCostume().getPixel(i - a1.second, j - a1.first) != '0'
					&& b->getCurrentCostume().getPixel(i - b1.second, j - b1.first) != '0')
					return true;
			}
		}
	}

	return false;
}