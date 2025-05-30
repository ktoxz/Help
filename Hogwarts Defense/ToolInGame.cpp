#pragma once
#define OEMRESOURCE
#include "ToolInGame.h"
#include "ToolConsole.h"
#include "std.h"

ToolInGame tool_in_game;
//===============================================================================================//
ToolInGame::ToolInGame() : Tool(), background("music/file_music.txt"), effect("music/file_sound.txt") {
	flag_exit = 0;
	size_input_buffer = 1;
	flag_bg_sound = -1;
	flag_sound_effect = -1;
	background.setFlagRepeat(1);
	global_speed = 1;
	coord_x_mouse_left_click = -1;
	coord_y_mouse_left_click = -1;
}

ToolInGame::~ToolInGame() {
	background.close();
	effect.close();
}

void ToolInGame::readInputEvent() {
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD prevMode;
	GetConsoleMode(hInput, &prevMode);
	SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

	INPUT_RECORD inputBuffer[128];
	DWORD eventsRead;
	ShowCursor(FALSE);  // Hide system cursor

	while (!flag_exit) {
		// Check for both mouse and keyboard events
		if (ReadConsoleInput(hInput, inputBuffer, 128, &eventsRead)) {
			for (DWORD i = 0; i < eventsRead; i++) {
				// Handle mouse events
				if (inputBuffer[i].EventType == MOUSE_EVENT) {
					MOUSE_EVENT_RECORD mouseEvent = inputBuffer[i].Event.MouseEvent;

					// Detect mouse button press
					if (mouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
						coord_x_mouse_left_click.store(mouseEvent.dwMousePosition.X);
						coord_y_mouse_left_click.store(mouseEvent.dwMousePosition.Y);
						cv_mouse.notify_all();  // Notify if needed
						cv_input.notify_all();
					}
					// Detect mouse movement
					else if (mouseEvent.dwEventFlags & MOUSE_MOVED) {
						coord_x_mouse_real_time = mouseEvent.dwMousePosition.X;
						coord_y_mouse_real_time = mouseEvent.dwMousePosition.Y;
					}
				}
				// Handle keyboard events
				else if (inputBuffer[i].EventType == KEY_EVENT) {
					KEY_EVENT_RECORD keyEvent = inputBuffer[i].Event.KeyEvent;
					if (keyEvent.bKeyDown) {  // Check if key is pressed
						int tmp = 0;
						// Check for special keys (ESC, Enter, Backspace)
						if (keyEvent.wVirtualKeyCode == VK_ESCAPE) tmp = KEY_ESC;
						else if (keyEvent.wVirtualKeyCode == VK_RETURN) tmp = 13;  // Enter key
						else if (keyEvent.wVirtualKeyCode == VK_BACK) tmp = 8;    // Backspace key

						// Check for arrow keys and map them to 2-5
						else if (keyEvent.wVirtualKeyCode == VK_LEFT) tmp = 3; // Left arrow
						else if (keyEvent.wVirtualKeyCode == VK_RIGHT) tmp = 4; // Right arrow
						else if (keyEvent.wVirtualKeyCode == VK_UP) tmp = 2;    // Up arrow
						else if (keyEvent.wVirtualKeyCode == VK_DOWN) tmp = 5;  // Down arrow

						// Check for alphanumeric keys (A-Z, 0-9)
						else {
							char keyChar = keyEvent.uChar.AsciiChar;
							if (keyChar >= 'A' && keyChar <= 'Z') tmp = keyChar;  // Uppercase A-Z
							else if (keyChar >= '0' && keyChar <= '9') tmp = keyChar;  // Numbers 0-9
							else if (keyChar >= 'a' && keyChar <= 'z') tmp = keyChar;  // Numbers 0-9
						}
						if (tmp != 0) {
							if (input_buffer.size() >= size_input_buffer) input_buffer.pop_front();
							input_buffer.push_back(tmp);
							if ((tmp >= 2 && tmp <= 5) || tmp == KEY_BACKSPACE || tmp == KEY_ENTER || tmp == KEY_ESC) {
								if (input_func_buffer.size() >= size_input_buffer) input_func_buffer.pop_front();
								input_func_buffer.push_back(tmp);
							}
							else {
								if (input_char_buffer.size() >= size_input_buffer) input_char_buffer.pop_front();
								input_char_buffer.push_back(tmp);
								if (tmp >= '0' && tmp <= '9') {
									if (input_digit_buffer.size() >= size_input_buffer) input_digit_buffer.pop_front();
									input_digit_buffer.push_back(tmp);
								}
								else if (isalpha(tmp)) {
									if (input_letter_buffer.size() >= size_input_buffer) input_letter_buffer.pop_front();
									input_letter_buffer.push_back(tmp);
								}
							}
							cv_keyboard.notify_all();
							cv_input.notify_all();
						}
					}
				}
			}
		}
	}

	// Restore console mode before exiting
	ShowCursor(TRUE);  // Show the cursor again
	SetConsoleMode(hInput, prevMode);
}

Figure* ToolInGame::getMouseEvent(const vector <Figure*>& list_figure) {
	{
		std::unique_lock<std::mutex> lock(cv_mtx);
		cv_mouse.wait(lock);
	}
	short cur_x_coord_mouse = coord_x_mouse_left_click.load(), cur_y_coord_mouse = coord_y_mouse_left_click.load();
	coord_x_mouse_left_click.store(-1);
	coord_y_mouse_left_click.store(-1);
	return checkFigureAtCoordOfMouse(cur_x_coord_mouse, cur_y_coord_mouse, list_figure);
}

short ToolInGame::getLetterKeyPress() {
	{
		std::unique_lock<std::mutex> lock(cv_keyboard_mtx);
		cv_keyboard.wait(lock);
	}
	if (input_letter_buffer.size() == 0) return 0;
	char res = input_letter_buffer.front();
	input_letter_buffer.pop_front();
	return res;
}

short ToolInGame::getDigitKeyPress() {
	{
		std::unique_lock<std::mutex> lock(cv_keyboard_mtx);
		cv_keyboard.wait(lock);
	}
	if (input_digit_buffer.size() == 0) return 0;
	char res = input_digit_buffer.front();
	input_digit_buffer.pop_front();
	return res;
}

short ToolInGame::getCharKeyPress() {
	{
		std::unique_lock<std::mutex> lock(cv_keyboard_mtx);
		cv_keyboard.wait(lock);
	}
	if (input_char_buffer.size() == 0) return 0;
	char res = input_char_buffer.front();
	input_char_buffer.pop_front();
	return res;
}

short ToolInGame::getFuncKeyPress() {
	{
		std::unique_lock<std::mutex> lock(cv_keyboard_mtx);
		cv_keyboard.wait(lock);
	}
	if (input_func_buffer.size() == 0) return 0;
	char res = input_func_buffer.front();
	input_func_buffer.pop_front();
	return res;
}

short ToolInGame::getKeyPress() {
	{
		std::unique_lock<std::mutex> lock(cv_keyboard_mtx);
		cv_keyboard.wait(lock);
	}
	if (input_buffer.size() == 0) return 0;
	char res = input_buffer.front();
	input_buffer.pop_front();
	return res;
}

Figure* ToolInGame::getInput(unordered_map<short, Figure*> exchange_table, const vector <Figure*>& list_figure) {
	Figure* button_click = nullptr;
	short key_press = 0;
	{
		std::unique_lock<std::mutex> lock(cv_input_mtx);
		cv_input.wait(lock);
	}
	short cur_x_coord_mouse = coord_x_mouse_left_click.load(), cur_y_coord_mouse = coord_y_mouse_left_click.load();
	if (cur_x_coord_mouse < 0 || cur_y_coord_mouse < 0) {
		if (input_buffer.size() > 0) {
			short tmp = input_buffer.front();
			input_buffer.pop_front();
			auto x = exchange_table.find(tmp);
			if (x != exchange_table.end()) return x->second;
			/*for (auto& x : exchange_table) {
				if (x.first == tmp) return x.second;
			}*/
		}
	}
	else {
		coord_x_mouse_left_click.store(-1);
		coord_y_mouse_left_click.store(-1);
		return checkFigureAtCoordOfMouse(cur_x_coord_mouse, cur_y_coord_mouse, list_figure);
	}
	return nullptr;
}

Figure* ToolInGame::checkFigureAtCoordOfMouse(short cur_x_coord_mouse, short cur_y_coord_mouse, const vector<Figure*>& list_figure)
{
	if (cur_x_coord_mouse < 0 || cur_y_coord_mouse < 0) return nullptr;
	short x1, y1, x2, y2;
	for (short i = (short)list_figure.size() - 1; i >= 0; i--) {
		if (list_figure[i] == nullptr) continue;
		if (list_figure[i]->getState() == 0) continue;
		x1 = list_figure[i]->getCurrentCoordinate().getCoordX();
		y1 = list_figure[i]->getCurrentCoordinate().getCoordY();
		x2 = x1 + list_figure[i]->getCurrentCostume().getNumberOfCol();
		y2 = y1 + list_figure[i]->getCurrentCostume().getNumberOfRow();
		if (cur_x_coord_mouse >= x1 && cur_x_coord_mouse <= x2 && cur_y_coord_mouse >= y1 && cur_y_coord_mouse <= y2) {
			return list_figure[i];
		}
	}
	return nullptr;
}

Figure* ToolInGame::getMouseMove(const vector<Figure*>& list_figure)
{
	short cur_x_coord_mouse = coord_x_mouse_real_time.load();
	short cur_y_coord_mouse = coord_y_mouse_real_time.load();
	if (cur_x_coord_mouse < 0 || cur_y_coord_mouse < 0) return nullptr;
	short x1, y1, x2, y2;
	for (short i = (short)list_figure.size() - 1; i >= 0; i--) {
		if (list_figure[i] == nullptr) continue;
		if (list_figure[i]->getState() == 0) continue;
		x1 = list_figure[i]->getCurrentCoordinate().getCoordX();
		y1 = list_figure[i]->getCurrentCoordinate().getCoordY();
		x2 = x1 + list_figure[i]->getCurrentCostume().getNumberOfCol();
		y2 = y1 + list_figure[i]->getCurrentCostume().getNumberOfRow();
		if (cur_x_coord_mouse >= x1 && cur_x_coord_mouse <= x2 && cur_y_coord_mouse >= y1 && cur_y_coord_mouse <= y2) {
			return list_figure[i];
		}
	}
	return nullptr;
}

void ToolInGame::exit()
{
	flag_exit = true;
	notifyAllConditionVariable();
}

void ToolInGame::notifyAllConditionVariable() {
	cv_bg_sound.notify_all(); // Notify all waiting threads to exit
	cv_sound_effect.notify_all(); // Notify all waiting threads to exit
	cv_mouse.notify_all();
	cv_keyboard.notify_all();
	cv_input.notify_all();
}

Graphic* ToolInGame::getCostumeLetter(char src)
{
	CharacterSystem& obj = CharacterSystem::getInstance("lettersystem/file_letter.txt");
	//return nullptr;
	return obj.getCharacter(src);
}

void ToolInGame::adjustBGSound() {
	while (!flag_exit) {
		{
			std::unique_lock<std::mutex> lock(cv_mtx_bg_sound);
			cv_bg_sound.wait(lock, [this] { return (flag_bg_sound != -1 || flag_exit); });
		}
		if (flag_exit) break;
		switch (flag_bg_sound.load())
		{
		case sound::FLAG_PLAY:
			background.play();
			break;
		case sound::FLAG_PAUSE:
			background.pause();
			break;
		case sound::FLAG_RESUME:
			background.resume();
			break;
		case sound::FLAG_INCVOL:
			background.increaseVolume();
			break;
		case sound::FLAG_DECVOL:
			background.decreaseVolume();
			break;
		case sound::FLAG_MUTE:
			background.setFlagMute(1);
			break;
		case sound::FLAG_UNMUTE:
			background.setFlagMute(0);
			break;
		case sound::FLAG_SWITCH_MUTE:
			if (background.getFlagMute() == true) background.setFlagMute(0);
			else background.setFlagMute(1);
			break;
		case sound::FLAG_CLOSE:
			background.close();
			break;
		case sound::FLAG_CHANGE:
			background.set(index_bg_sound.load());
			index_bg_sound.store(0);
			break;
		default:
			break;
		}
		flag_bg_sound.store(-1);
		cv_bg_sound.notify_all();
	}
}

void ToolInGame::adjustFlagBGSound(short flag, short index) {
	bool check = 0;
	switch (flag)
	{
	case sound::FLAG_PLAY:
	case sound::FLAG_PAUSE:
	case sound::FLAG_RESUME:
	case sound::FLAG_INCVOL:
	case sound::FLAG_DECVOL:
	case sound::FLAG_MUTE:
	case sound::FLAG_UNMUTE:
	case sound::FLAG_SWITCH_MUTE:
	case sound::FLAG_CLOSE:
		flag_bg_sound.store(flag);
		check = 1;
		break;
	case sound::FLAG_CHANGE:
		flag_bg_sound.store(flag);
		index_bg_sound.store(index);
		check = 1;
		break;
	default:
		break;
	}
	if (check) {
		cv_bg_sound.notify_all();
		std::unique_lock<std::mutex> lock(cv_mtx_bg_sound);
		cv_bg_sound.wait(lock, [this] { return (flag_bg_sound == -1 || flag_exit); });
	}
}

void ToolInGame::adjustSoundEffect() {
	while (!flag_exit) {
		{
			std::unique_lock<std::mutex> lock(cv_mtx_sound_effect);
			cv_sound_effect.wait(lock, [this] { return (flag_sound_effect != -1 || flag_exit); });
		}
		if (flag_exit) break;
		switch (flag_sound_effect.load())
		{
		case sound::FLAG_PLAY:
			effect.play();
			break;
		case sound::FLAG_PAUSE:
			effect.pause();
			break;
		case sound::FLAG_RESUME:
			effect.resume();
			break;
		case sound::FLAG_INCVOL:
			effect.increaseVolume();
			break;
		case sound::FLAG_DECVOL:
			effect.decreaseVolume();
			break;
		case sound::FLAG_MUTE:
			effect.setFlagMute(1);
			break;
		case sound::FLAG_UNMUTE:
			effect.setFlagMute(0);
			break;
		case sound::FLAG_SWITCH_MUTE:
			if (effect.getFlagMute() == true) effect.setFlagMute(0);
			else effect.setFlagMute(1);
			break;
		case sound::FLAG_CLOSE:
			effect.close();
			break;
		case sound::FLAG_CHANGE:
			effect.set(index_sound_effect.load());
			index_sound_effect.store(0);
			break;
		default:
			break;
		}
		flag_sound_effect = -1;
		cv_sound_effect.notify_all();
	}
}

void ToolInGame::adjustFlagSoundEffect(short flag, short index) {
	bool check = 0;
	switch (flag)
	{
	case sound::FLAG_PLAY:
	case sound::FLAG_PAUSE:
	case sound::FLAG_RESUME:
	case sound::FLAG_INCVOL:
	case sound::FLAG_DECVOL:
	case sound::FLAG_MUTE:
	case sound::FLAG_UNMUTE:
	case sound::FLAG_SWITCH_MUTE:
	case sound::FLAG_CLOSE:
		flag_sound_effect.store(flag);
		check = 1;
		break;
	case sound::FLAG_CHANGE:
		flag_sound_effect.store(flag);
		index_sound_effect.store(index);
		check = 1;
		break;
	default:
		break;
	}
	if (check) {
		cv_sound_effect.notify_all();
		std::unique_lock<std::mutex> lock(cv_mtx_sound_effect);
		cv_sound_effect.wait(lock, [this] { return (flag_sound_effect == -1 || flag_exit); });
	}
}

short ToolInGame::getVolumeBGMusic() const {
	return background.getVolume() / 200;
}

short ToolInGame::getVolumeSoundEffect() const
{
	return effect.getVolume() / 200;
}

void ToolInGame::setGlobalSpeed(short speed) {
	global_speed = speed;
}

short ToolInGame::getGlobalSpeed() {
	return global_speed;
}

void ToolInGame::runClock() {
	time_mili.store(0);
	while (flag_clock.load() != 1) {
		while (flag_clock.load() == -1) tool_console.sleepMillisecond(10);
		auto st = chrono::high_resolution_clock::now();
		tool_console.sleepMillisecond(10);
		auto en = chrono::high_resolution_clock::now();
		chrono::duration<double, milli> dur = en - st;
		time_mili.store(dur.count() * global_speed + time_mili.load());
	}
	time_mili.store(0);
}

void ToolInGame::setFlagClock(char flag) {
	flag_clock.store(flag);
}

long long ToolInGame::getClock() {
	return time_mili.load();
}

void ToolInGame::costumizeMouseIcon() {

	while (!flag_exit) {
		RECT consoleRect;
		GetWindowRect(console_window, &consoleRect);

		// Get current mouse position
		POINT mousePos;
		GetCursorPos(&mousePos);


		// Check if the mouse position is inside the console window bounds
		if (mousePos.x >= consoleRect.left && mousePos.x <= consoleRect.right &&
			mousePos.y >= consoleRect.top && mousePos.y <= consoleRect.bottom) {
			HCURSOR hCursor = LoadCursorFromFile(L"mouse/mouse1.cur");
			SetSystemCursor(hCursor, OCR_NORMAL);
			DestroyCursor(hCursor);
		}
		else {
			HCURSOR defaultCursor = LoadCursorFromFile(L"mouse/aero_arrow.cur");
			SetSystemCursor(defaultCursor, OCR_NORMAL);
			DestroyCursor(defaultCursor);
		}
		Sleep(500);
	}
	HCURSOR defaultCursor = LoadCursorFromFile(L"mouse/aero_arrow.cur");
	SetSystemCursor(defaultCursor, OCR_NORMAL);
	DestroyCursor(defaultCursor);

	return;
	mouse = new Figure(0, 0, "mouse/file_mouse.txt", 1);
	while (!flag_exit) {
		mouse->setCurrentCoordinate(coord_x_mouse_real_time.load(), coord_y_mouse_real_time.load());
	}
	delete mouse;
}

short ToolInGame::getCoordXMouseRealTime() {
	return coord_x_mouse_real_time;
}

short ToolInGame::getCoordYMouseRealTime() {
	return coord_y_mouse_real_time;
}
