#pragma once
#include "std.h"

using namespace std;

class Tool {
protected:
	HWND console_window;
	HMENU system_menu;
	HANDLE console_output;
	short width_console_screen, height_console_screen;

public:
	Tool();
	short getWidthConsoleScreen();
	short getHeightConsoleScreen();
};
