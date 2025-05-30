#include <iostream>
#include <string>
#include "Graphic.h"
#include "Coordinate.h"
#include "ToolConsole.h"
#include <conio.h>


using namespace std;

int main() {
    ToolConsole toolconsole;
    Graphic graphic;
    Coordinate coordinate;

    toolconsole.setUpConsoleScreen();
    graphic.setPixel("Home/background.txt");

    coordinate.setCoordX(0);
    coordinate.setCoordY(0);

    toolconsole.drawToBuffer(graphic, coordinate, true); 
    toolconsole.showBuffer();

    _getch();


}
