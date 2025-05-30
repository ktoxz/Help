#pragma once
#include "std.h"
class Coordinate {
private:
	short coord_x, coord_y;

public:
	Coordinate();
	Coordinate(short, short);

	void operator=(const Coordinate&);
	float calcDistance(const Coordinate&) const;
	void setCoordX(short);

	void setCoordY(short);
	short getCoordX() const;
	short getCoordY() const;

	friend class Button;
};