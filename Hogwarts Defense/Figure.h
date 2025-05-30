#pragma once
#include "std.h"
#include "Graphic.h"
#include "Coordinate.h"

class Figure {
protected:
	Graphic* costume;
	short n_costume, cur_costume;
	Coordinate cur_coord;
	double scale_factor;
	atomic <bool> flag_stop, flag_pause, state, direction;
public:
	Figure();
	Figure(short, short, string, bool = 0);
	virtual ~Figure();
	void setCostume(string);
	Graphic& getCostumeAtIndex(short = 0);
	Graphic& getCurrentCostume();

	void setNumberOfCostume(short);
	short getNumberOfCostume() const;

	short getIndexCurrentCostume();
	void setIndexCurrentCostume(short);

	void setCurrentCoordinate(short, short);
	void setCurrentCoordinate(const Coordinate&);
	Coordinate& getCurrentCoordinate();
	Coordinate getCurrentCenterCoordinate();

	void setState(bool);
	bool getState() const;

	void setFlagStop(bool);
	bool getFlagStop() const;

	void setFlagPause(bool);
	bool getFlagPause() const;

	void setScaleFactor(double src);
	double getScaleFactor() const;

	void setDirection(bool src);
	bool getDirection();

	bool checkCollision(Figure* b);
};