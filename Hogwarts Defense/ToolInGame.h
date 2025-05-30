#pragma once
#include "Tool.h"
#include "CharacterSystem.h"
#include "Sound.h"
#include "Coordinate.h"
#include "Figure.h"

//FLAG FOR SOUND
namespace sound {
	constexpr auto FLAG_PLAY = 0;
	constexpr auto FLAG_PAUSE = 1;
	constexpr auto FLAG_RESUME = 2;
	constexpr auto FLAG_INCVOL = 3;
	constexpr auto FLAG_DECVOL = 4;
	constexpr auto FLAG_MUTE = 5;
	constexpr auto FLAG_UNMUTE = 6;
	constexpr auto FLAG_SWITCH_MUTE = 7;
	constexpr auto FLAG_CLOSE = 8;
	constexpr auto FLAG_CHANGE = 9;


	constexpr auto SE_HITDAMAGE = 0;
	constexpr auto SE_LASER = 1;
	constexpr auto SE_BOOM = 2;
	constexpr auto SE_BUTTON = 3;
	constexpr auto SE_COIN = 4;

	constexpr auto BG_STAGE = 1;
	constexpr auto BG_MENU = 0;
}

//KEY PRESS
constexpr int KEY_UP = 72;
constexpr int KEY_DOWN = 80;
constexpr int KEY_LEFT = 75;
constexpr int KEY_RIGHT = 77;
constexpr int KEY_ESC = 27;
constexpr int KEY_ENTER = 13;
constexpr int KEY_BACKSPACE = 8;


class ToolInGame : public Tool {
private:
	Sound background, effect;
	// variable for sound
	atomic <short> flag_bg_sound, flag_sound_effect, index_bg_sound, index_sound_effect;
	condition_variable cv_bg_sound; // Condition variable to signal key press
	mutex cv_mtx_bg_sound; // Mutex for condition variable
	condition_variable cv_sound_effect; // Condition variable to signal key press
	mutex cv_mtx_sound_effect; // Mutex for condition variable
	bool flag_exit;
	// variable for get key press
	mutex cv_mtx;

	deque <char> input_buffer, input_char_buffer, input_func_buffer, input_digit_buffer, input_letter_buffer;
	short size_input_buffer;
	condition_variable cv_keyboard;
	mutex cv_keyboard_mtx;
	//
	short global_speed;
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// key
	atomic <short> coord_x_mouse_left_click, coord_y_mouse_left_click;
	atomic <short> coord_x_mouse_real_time, coord_y_mouse_real_time;
	condition_variable cv_mouse;
	mutex cv_mouse_mtx;
	// 
	condition_variable cv_input;
	mutex cv_input_mtx;

	// Clock
	atomic<char> flag_clock;
	atomic<long long> time_mili;
public:
	Figure* mouse;
	ToolInGame();
	~ToolInGame();

	void readInputEvent();
	Figure* getMouseEvent(const vector <Figure*>& list_figure);
	short getCharKeyPress();
	short getDigitKeyPress();
	short getLetterKeyPress();
	short getFuncKeyPress();
	short getKeyPress();
	Figure* getInput(unordered_map<short, Figure*> exchange_table, const vector <Figure*>& list_figure);
	Figure* checkFigureAtCoordOfMouse(short cur_x_coord_mouse, short cur_y_coord_mouse, const vector <Figure*>& list_figure);
	Figure* getMouseMove(const vector <Figure*>& list_figure);

	void exit();
	void notifyAllConditionVariable();
	Graphic* getCostumeLetter(char src);
	void adjustFlagBGSound(short flag, short index = 0);
	void adjustBGSound();
	void adjustFlagSoundEffect(short flag, short index = 0);
	void adjustSoundEffect();

	short getVolumeBGMusic() const;
	short getVolumeSoundEffect() const;

	void setGlobalSpeed(short speed);
	short getGlobalSpeed();

	void runClock();
	void setFlagClock(char flag);
	long long getClock();

	void costumizeMouseIcon();
	short getCoordXMouseRealTime();
	short getCoordYMouseRealTime();
};

extern ToolInGame tool_in_game;

