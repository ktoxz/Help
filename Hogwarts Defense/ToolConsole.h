#pragma once
#include "Tool.h"

class Figure;
class Coordinate;
class Graphic;

class ToolConsole : public Tool {
private:
	bool flag_exit;
	short fps;
	short cur_buffer;
	HANDLE screen_buffers[2];
	CHAR_INFO* screen;
public:
	ToolConsole();
	~ToolConsole();

	void setMaximumWindow();
	void disableResizeWindow();
	void disableCtrButton(bool, bool, bool);
	void setWindowSize(SHORT, SHORT, HANDLE);
	void setScreenBufferSize(HANDLE);
	void hideScrollBar();
	void setUpConsoleScreen();
	void setPalleteColor(HANDLE);
	void setSizeOfPixel(HANDLE, short);
	void initScreenBuffers();

	void swapBuffers();
	void drawToBuffer(Graphic&, Coordinate&, bool);
	void drawWithScaleToBuffer(Graphic&, Coordinate&, const double&, bool);
	void showBuffer();
	void runConsole(vector <Figure*>&);

	bool getFlagExit();
	void setFlagExit(bool);

	void sleepMillisecond(short);
	void sleepNanosecond(long long);
};

extern ToolConsole tool_console;