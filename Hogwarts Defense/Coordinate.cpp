#include "Coordinate.h"

Coordinate::Coordinate() : coord_x(0), coord_y(0) {}

Coordinate::Coordinate(short src_x, short src_y) {
	setCoordX(src_x);
	setCoordY(src_y);
}

void Coordinate::operator=(const Coordinate& src) {
	coord_x = src.coord_x;
	coord_y = src.coord_y;
}

short Coordinate::getCoordX() const {
	return coord_x;
}

short Coordinate::getCoordY() const {
	return coord_y;
}

void Coordinate::setCoordX(short x) {
	coord_x = x;
}

void Coordinate::setCoordY(short y) {
	coord_y = y;
}

float Coordinate::calcDistance(const Coordinate& b) const
{
	short c = b.coord_x - coord_x;
	short d = b.coord_y - coord_y;
	return sqrt(c * c + d * d);
}
