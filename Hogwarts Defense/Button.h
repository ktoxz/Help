
#pragma once
#include "Figure.h"

class Graphic;
class Coordinate;

class Button : public Figure {
private:
public:
	using Figure::Figure;
	void bottomUpEffect(short time_delay);
	void topDownEffect(short time_delay);
	void appearEffect(short time_delay);
	void slide(short x1, short y1, short x2, short y2, short time_delay);
	void scaleRightTop(double scale, short time_delay);
	void scaleRightBottom(double scale, short time_delay);
	void scaleLeftTop(double scale, short time_delay);
	void scaleLeftBottom(double scale, short time_delay);
	void slideWithScale(short x1, short y1, short x2, short y2, double scale, short time_delay);
};