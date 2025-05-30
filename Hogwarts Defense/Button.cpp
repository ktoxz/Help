#include "Button.h"
#include "Graphic.h"
#include "utility.h"

void Button::bottomUpEffect(short time_delay) {
	Coordinate tmp(cur_coord.getCoordX(), cur_coord.getCoordY());
	state = 0;
	short des_y = cur_coord.getCoordY();
	cur_coord.setCoordY(tool_console.getHeightConsoleScreen());
	short cur_y = cur_coord.getCoordY();
	state = 1;
	while (cur_y >= des_y) {
		cur_coord.setCoordY(cur_y);
		cur_y--;
		tool_console.sleepMillisecond(time_delay);
	}
	cur_coord.setCoordY(tmp.getCoordY());
}

void Button::topDownEffect(short time_delay) {
	Coordinate tmp(cur_coord.getCoordX(), cur_coord.getCoordY());
	state = 0;
	short des_y = cur_coord.getCoordY();
	cur_coord.setCoordY(0 - getImage().getNumberOfCol());
	short cur_y = cur_coord.getCoordY();
	state = 1;
	while (cur_y != des_y) {
		cur_coord.setCoordY(cur_y);
		cur_y++;
		tool_console.sleepMillisecond(time_delay);
	}
	cur_coord.setCoordY(tmp.getCoordY());
}

void Button::appearEffect(short time_delay) {
	short n_col = getImage().getNumberOfCol();
	for (int i = 0; i <= n_col; i++) {
		getImage().setNumberOfCol(i);
		tool_console.sleepMillisecond(time_delay);
	}
}

void Button::slide(short x1, short y1, short x2, short y2, short time_delay) {
	while (!flag_stop && (x1 != x2 || y1 != y2)) {
		if (x1 < x2) x1++;
		else if (x1 > x2) x1--;
		if (y1 < y2) y1++;
		else if (y1 > y2) y1--;
		cur_coord.coord_x = x1;
		cur_coord.coord_y = y1;
		tool_console.sleepNanosecond(500);
	}
}

void Button::scaleRightTop(double scale, short time_delay) {
	double cur_scale = this->getScaleFactor();
	short ori_height = this->getImage().getNumberOfRow();
	short new_height, old_height = short(ori_height * cur_scale);

	double delta;
	if (cur_scale < scale) delta = 0.5;
	else delta = -0.5;
	short num_scale = short(abs(cur_scale - scale) / abs(delta) + 1);

	while (num_scale--) {
		this->setScaleFactor(cur_scale);
		new_height = short(ori_height * cur_scale);
		short coord_y = this->getCoord().getCoordY();
		if (delta > 0)
			this->getCoord().setCoordY(coord_y - abs(new_height - old_height));
		else
			this->getCoord().setCoordY(coord_y + abs(new_height - old_height));
		old_height = new_height;
		cur_scale = cur_scale + delta;
		tool_console.sleepMillisecond(time_delay);
	}
}

void Button::scaleRightBottom(double scale, short time_delay)
{
	double cur_scale = this->getScaleFactor();

	double delta;
	if (cur_scale < scale) delta = 0.5;
	else delta = -0.5;
	short num_scale = short(abs(cur_scale - scale) / abs(delta) + 1);

	while (num_scale--) {
		this->setScaleFactor(cur_scale);
		short coord_y = this->getCoord().getCoordY();
		cur_scale = cur_scale + delta;
		tool_console.sleepMillisecond(time_delay);
	}
}

void Button::scaleLeftTop(double scale, short time_delay)
{
	double cur_scale = this->getScaleFactor();
	short ori_height = this->getImage().getNumberOfRow();
	short ori_width = this->getImage().getNumberOfCol();
	short new_height, old_height = short(ori_height * cur_scale);
	short new_width, old_width = short(ori_width * cur_scale);

	double delta;
	if (cur_scale < scale) delta = 0.5;
	else delta = -0.5;
	short num_scale = short(abs(cur_scale - scale) / abs(delta) + 1);

	while (num_scale--) {
		this->setScaleFactor(cur_scale);
		new_height = short(ori_height * cur_scale);
		new_width = short(ori_width * cur_scale);
		short coord_y = this->getCoord().getCoordY();
		short coord_x = this->getCoord().getCoordX();
		if (delta > 0) {
			this->getCoord().setCoordY(coord_y - abs(new_height - old_height));
			this->getCoord().setCoordX(coord_x - abs(new_width - old_width));
		}
		else {
			this->getCoord().setCoordY(coord_y + abs(new_height - old_height));
			this->getCoord().setCoordX(coord_x + abs(new_width - old_width));
		}
		old_height = new_height;
		old_width = new_width;
		cur_scale = cur_scale + delta;
		tool_console.sleepMillisecond(time_delay);
	}
}

void Button::scaleLeftBottom(double scale, short time_delay)
{
	double cur_scale = this->getScaleFactor();
	short ori_width = this->getImage().getNumberOfRow();
	short new_width, old_width = short(ori_width * cur_scale);

	double delta;
	if (cur_scale < scale) delta = 0.5;
	else delta = -0.5;
	short num_scale = short(abs(cur_scale - scale) / abs(delta) + 1);

	while (num_scale--) {
		this->setScaleFactor(cur_scale);
		new_width = short(ori_width * cur_scale);
		short coord_x = this->getCoord().getCoordX();
		if (delta > 0)
			this->getCoord().setCoordX(coord_x - abs(new_width - old_width));
		else
			this->getCoord().setCoordX(coord_x + abs(new_width - old_width));
		old_width = new_width;
		cur_scale = cur_scale + delta;
		tool_console.sleepMillisecond(time_delay);
	}
}

void Button::slideWithScale(short x1, short y1, short x2, short y2, double scale, short time_delay) {
	short delta_x = abs(x1 - x2);
	short delta_y = abs(y1 - y2);
	double delta_scale;
	if (scale == this->scaleFact) delta_scale = 0;
	else if (scale > this->scaleFact) delta_scale = 0.1;
	else delta_scale = -0.1;

	short step = abs(scale - this->scaleFact);
	if (delta_scale == 0) step = -1;
	else {
		step = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) / abs(scale - this->scaleFact) * abs(delta_scale);
	}
	short count = 0;
	while (!flag_stop && (x1 != x2 || y1 != y2)) {
		if (x1 < x2) x1++;
		else if (x1 > x2) x1--;
		if (y1 < y2) y1++;
		else if (y1 > y2) y1--;
		cur_coord.coord_x = x1;
		cur_coord.coord_y = y1;
		tool_console.sleepMillisecond(time_delay);
		if (count == step && scaleFact < scale) {
			count = 0;
			this->scaleFact += 0.1;
		}
		else count++;
	}
	scaleFact = scale;
}


