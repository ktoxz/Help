#pragma once
#define OEMRESOURCE
#include "ToolConsole.h"
#include "ToolInGame.h"
#include "Figure.h"
#include <winuser.h> 
#pragma comment (lib, "winmm.lib")
ToolConsole tool_console;

//===============================================================================================//
ToolConsole::ToolConsole() : Tool() {
    flag_exit = 0;
    fps = 240;
    cur_buffer = 0;
    screen = new CHAR_INFO[width_console_screen * height_console_screen];
    for (short y = 0; y < height_console_screen; y++) {
        for (short x = 0; x < width_console_screen; x++) {
            screen[x + width_console_screen * y].Char.UnicodeChar = 32;
            screen[x + width_console_screen * y].Attributes = 0;
        }
    }
}

ToolConsole::~ToolConsole() {
    CloseHandle(screen_buffers[0]);
    CloseHandle(screen_buffers[1]);
    if (screen)
        delete[] screen;
    screen = nullptr;
}

void ToolConsole::disableCtrButton(bool Close, bool Min, bool Max) {

    if (Close == 1)
    {
        DeleteMenu(system_menu, SC_CLOSE, MF_BYCOMMAND);
    }
    if (Min == 1)
    {
        DeleteMenu(system_menu, SC_MINIMIZE, MF_BYCOMMAND);
    }
    if (Max == 1)
    {
        DeleteMenu(system_menu, SC_MAXIMIZE, MF_BYCOMMAND);
    }
}

void ToolConsole::hideScrollBar()
{
    ShowScrollBar(console_window, SB_BOTH, false);
}


void ToolConsole::setMaximumWindow() {
    ShowWindow(console_window, SW_MAXIMIZE);
}

void ToolConsole::setWindowSize(SHORT width, SHORT height, HANDLE buffer)
{
    RECT r;
    GetWindowRect(console_window, &r); 

    MoveWindow(console_window, 0, 0, width, height, TRUE);
}

void ToolConsole::setScreenBufferSize(HANDLE buffer)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SHORT columns, rows;

    GetConsoleScreenBufferInfo(buffer, &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    COORD NewSize;
    NewSize.X = columns;
    NewSize.Y = rows;

    SetConsoleScreenBufferSize(buffer, NewSize);
}

void ToolConsole::disableResizeWindow()
{
    SetWindowLong(console_window, GWL_STYLE, GetWindowLong(console_window, GWL_STYLE) & ~WS_SIZEBOX);
}

void ToolConsole::setUpConsoleScreen() {
    console_window = GetConsoleWindow();
    Sleep(50);

    system_menu = GetSystemMenu(console_window, FALSE);

    disableCtrButton(0, 0, 1);
    initScreenBuffers();
    hideScrollBar();
    setMaximumWindow();
    disableResizeWindow();
}



void ToolConsole::setSizeOfPixel(HANDLE buffer, short size_pixel) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = cfi.dwFontSize.Y = size_pixel;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas"); 
    SetCurrentConsoleFontEx(buffer, FALSE, &cfi);
}

void ToolConsole::setPalleteColor(HANDLE buffer) {
    CONSOLE_SCREEN_BUFFER_INFOEX csbiex;

    csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    GetConsoleScreenBufferInfoEx(buffer, &csbiex);

    ifstream fi("color.txt");
    for (int i = 0; i < 16; i++) {
        int r, g, b;
        fi >> r >> g >> b;
        csbiex.ColorTable[i] = RGB(r, g, b);
    }
    fi.close();
    SetConsoleScreenBufferInfoEx(buffer, &csbiex);
}

void ToolConsole::initScreenBuffers() {
    SMALL_RECT rect = { 0, 0, 1000, 1500 };
    for (int i = 0; i < 2; ++i) {
        screen_buffers[i] = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(screen_buffers[i]);
        setPalleteColor(screen_buffers[i]);
        setSizeOfPixel(screen_buffers[i], 5);
        setWindowSize(width_console_screen * 5 - 20, height_console_screen * 5 + 10, screen_buffers[i]);
        setScreenBufferSize(screen_buffers[i]);
        disableResizeWindow();
    }
}


void ToolConsole::swapBuffers() {
    cur_buffer = 1 - cur_buffer;
    SetConsoleActiveScreenBuffer(screen_buffers[cur_buffer]);
}

void ToolConsole::drawToBuffer(Graphic& costume, Coordinate& coord, bool direction) {
    short n_row = costume.getNumberOfRow();
    short n_col = costume.getNumberOfCol();
    short st_coord_x = coord.getCoordX();
    short st_coord_y = coord.getCoordY();
    if (direction == 0) goto DirectionRightToLeft;
    else goto DirectionLeftToRight;
DirectionLeftToRight:
    {
        for (short y = st_coord_y; y < n_row + st_coord_y; y++) {
            for (short x = st_coord_x; x < n_col + st_coord_x; x++) {
                if (x < 0 || x >= width_console_screen ||
                    y < 0 || y >= height_console_screen) {
                    continue;
                }
                char color = costume.getPixel(y - st_coord_y, x - st_coord_x) - 'a';
                if (color >= 0 && color < 16) {
                    screen[x + width_console_screen * y].Char.UnicodeChar = 32;
                    screen[x + width_console_screen * y].Attributes = 16 * color;
                }

            }
        }
    }
    return;
DirectionRightToLeft:
    {
        for (short y = st_coord_y; y < n_row + st_coord_y; y++) {
            for (short x = st_coord_x; x < n_col + st_coord_x; x++) {
                if (x < 0 || x >= width_console_screen ||
                    y < 0 || y >= height_console_screen) {
                    continue;
                }
                char color = costume.getPixel(y - st_coord_y, x - st_coord_x) - 'a';
                if (color >= 0 && color < 16) {
                    screen[(n_col + st_coord_x - 1 - x + st_coord_x) + width_console_screen * y].Char.UnicodeChar = 32;
                    screen[(n_col + st_coord_x - 1 - x + st_coord_x) + width_console_screen * y].Attributes = 16 * color;
                }

            }
        }
    }
}

void ToolConsole::drawWithScaleToBuffer(Graphic& costume, Coordinate& coord, const double& scale_factor, bool direction) {
    short n_row = costume.getNumberOfRow();
    short n_col = costume.getNumberOfCol();
    short st_coord_x = coord.getCoordX();
    short st_coord_y = coord.getCoordY();
    if (direction == 0) goto DirectionRightToLeft;
    else goto DirectionLeftToRight;
DirectionLeftToRight:
    {
        for (short y = st_coord_y; y < n_row * scale_factor + st_coord_y; y++) {
            for (short x = st_coord_x; x < n_col * scale_factor + st_coord_x; x++) {
                if (x < 0 || x >= width_console_screen ||
                    y < 0 || y >= height_console_screen) {
                    continue;
                }

                double tmp_y = (y - st_coord_y) / scale_factor;
                double tmp_x = (x - st_coord_x) / scale_factor;


                int y1 = static_cast<int>(tmp_y),
                    x1 = static_cast<int>(tmp_x),
                    y2 = min(y1 + 1, n_row - 1),
                    x2 = min(x1 + 1, n_col - 1);
                double diff_y = tmp_y - y1;
                double diff_x = tmp_x - x1;

                char color = costume.getPixel((diff_y < 0.5 ? y1 : y2), (diff_x < 0.5 ? x1 : x2)) - 'a';
                if (color >= 0 && color < 16) {
                    screen[x + width_console_screen * y].Char.UnicodeChar = 32;
                    screen[x + width_console_screen * y].Attributes = 16 * color;
                }

            }
        }
    }
    return;
DirectionRightToLeft:
    {
        for (short y = st_coord_y; y < n_row * scale_factor + st_coord_y; y++) {
            for (short x = st_coord_x; x < n_col * scale_factor + st_coord_x; x++) {
                if (x < 0 || x >= width_console_screen ||
                    y < 0 || y >= height_console_screen) {
                    continue;
                }

                double tmp_y = (y - st_coord_y) / scale_factor;
                double tmp_x = (x - st_coord_x) / scale_factor;


                int y1 = static_cast<int>(tmp_y),
                    x1 = static_cast<int>(tmp_x),
                    y2 = min(y1 + 1, n_row - 1),
                    x2 = min(x1 + 1, n_col - 1);
                double diff_y = tmp_y - y1;
                double diff_x = tmp_x - x1;

                char color = costume.getPixel((diff_y < 0.5 ? y1 : y2), (diff_x < 0.5 ? x1 : x2)) - 'a';
                if (color >= 0 && color < 16) {
                    screen[(n_col + st_coord_x - 1 - x + st_coord_x) + width_console_screen * y].Char.UnicodeChar = 32;
                    screen[(n_col + st_coord_x - 1 - x + st_coord_x) + width_console_screen * y].Attributes = 16 * color;
                }

            }
        }
    }
}

void ToolConsole::showBuffer() {
    static COORD start_pos = { 0, 0 };
    static SMALL_RECT SBregion = { start_pos.X, start_pos.Y, start_pos.X + width_console_screen - 1  , start_pos.Y + height_console_screen - 1 };
    WriteConsoleOutput(screen_buffers[1 - cur_buffer], screen, { width_console_screen, height_console_screen }, start_pos, &SBregion);
    swapBuffers();
}

void ToolConsole::runConsole(vector<Figure*>& list_figure) {
    while (!flag_exit) {
        auto st = chrono::high_resolution_clock::now();
        for (auto& x : list_figure) {
            if (x && x->getState()) {
                if (x->getScaleFactor() == 1.0)
                    drawToBuffer(x->getCurrentCostume(), x->getCurrentCoordinate(), x->getDirection());
                else
                    drawWithScaleToBuffer(x->getCurrentCostume(), x->getCurrentCoordinate(), x->getScaleFactor(), x->getDirection());
            }
        }
        auto en = chrono::high_resolution_clock::now();
        chrono::duration<double, nano> dur = en - st;
        showBuffer();
        sleepNanosecond(static_cast<long long>(1e9 / fps - dur.count()));
    }
}

bool ToolConsole::getFlagExit()
{
    return flag_exit;
}

void ToolConsole::setFlagExit(bool src)
{
    flag_exit = src;
}

void ToolConsole::sleepMillisecond(short src) {
    short speed = tool_in_game.getGlobalSpeed();
    if (src / speed == 0) sleepNanosecond((long long)src * 1000000);
    else this_thread::sleep_for(chrono::milliseconds(src / speed));
}

void ToolConsole::sleepNanosecond(long long src)
{
    short speed = tool_in_game.getGlobalSpeed();
    this_thread::sleep_for(chrono::nanoseconds(src / speed));
}

