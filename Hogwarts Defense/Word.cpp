#include "Word.h"
#include "utility.h"


Word::Word(short max_len) : Figure() {
	this->max_len = max_len;
	n_costume = 1;
	costume = new Graphic[1];
	costume->pixel = new char* [5];
	for (int i = 0; i < 5; i++) {
		(costume->pixel)[i] = new char;
		(costume->pixel)[i][0] = '0';
	}
	costume->n_col = 1;
	costume->n_row = 5;
	word = "";
	color = BLACK;
}

Word::~Word()
{
}

Word::Word(const string& src) : Word() {
	for (int i = 0; i < src.length(); i++)
		addCharacter(src[i]);
	return;
	for (auto& x : src) addCharacter(x);
}

string Word::getWord() {
	return word;
}

char Word::getLastCharacter() {
	if (word.size() == 0) return 0;
	return *(word.end() - 1);
}

void Word::addCharacter(char new_char) {
	Graphic* new_character = tool_in_game.getCostumeLetter(new_char);
	if (new_character == nullptr) {
		state = 1;
		return;
	}

	word.push_back(new_char);

	short n_row_cur_word = costume->n_row,
		n_col_cur_word = costume->n_col;
	short n_row_new_word = n_row_cur_word,
		n_col_new_word = n_col_cur_word + new_character->getNumberOfCol() + 1;
	char** res = new char* [n_row_new_word];

	//res[0].pixel = new char* [n_row_new_word];
	for (short i = 0; i < n_row_new_word; i++) {
		res[i] = new char[n_col_new_word];
		if (this->costume != nullptr) {
			for (short j = 0; j < n_col_cur_word; j++)
				res[i][j] = costume[0].pixel[i][j];
		}
	}
	for (short i = 0; i < n_row_new_word; i++) {
		for (short j = n_col_cur_word; j < n_col_new_word - 1; j++) {
			char tmp = (new_character->pixel)[i][j - n_col_cur_word];
			if (tmp != '0') tmp = color;
			res[i][j] = tmp;
		}
	}

	for (short i = 0; i < n_row_new_word; i++) {
		res[i][n_col_new_word - 1] = '0';
	}

	// Assign new costume
	//tool_console.sleepMillisecond(9);
	this->clear();
	costume->pixel = res;
	costume->n_col = n_col_new_word;
	costume->n_row = n_row_new_word;
}

void Word::removeCharacter() {
	ofstream fo("output.txt");
	fo << word;
	fo.close();
	if (word.size() == 0) return;

	char last_char = getLastCharacter();
	word.erase(word.end() - 1, word.end());
	Graphic* last_character = tool_in_game.getCostumeLetter(last_char);
	if (last_character == nullptr) {
		state = 1;
		return;
	}
	short n_row_new_word = costume->n_row,
		n_col_new_word = costume->n_col - last_character->getNumberOfCol() - 1;
	char** res = new char* [n_row_new_word];
	for (short i = 0; i < n_row_new_word; i++) {
		res[i] = new char[n_col_new_word];
		for (short j = 0; j < n_col_new_word; j++)
			res[i][j] = (costume->pixel)[i][j];
	}
	//tool_console.sleepMillisecond(9);
	clear();
	costume->pixel = res;
	costume->n_col = n_col_new_word;
	costume->n_row = n_row_new_word;
}

void Word::clear() {
	if (costume->pixel) {
		for (short i = 0; i < costume->n_row; i++) {
			delete[] costume->pixel[i];
		}
		delete[] costume->pixel;
	}
	return;

}

void Word::setCharacter(char src) {
	//backspace
	if (src == 8) removeCharacter();
	//others character
	if (word.length() >= max_len) return;
	else addCharacter(src);
}

void Word::setString(const string& src) {
	eraseAllCharacter();
	for (auto& x : src) addCharacter(x);
}

void Word::setColor(char color) {
	if (color < BLACK || color > CYAN) return;
	this->color = color;
	changeColor();
}

void Word::changeColor() {
	for (int i = 0; i < costume->n_row; i++)
		for (int j = 0; j < costume->n_col; j++)
			if (costume->pixel[i][j] != '0') costume->pixel[i][j] = color;
}

void Word::setMiddleCoordinate(short mid_coord_x, short coord_y) {
	setCurrentCoordinate(mid_coord_x - costume->n_col / 2, coord_y);
}

void Word::eraseAllCharacter() {
	clear();
	costume->pixel = new char* [5];
	for (int i = 0; i < 5; i++) {
		(costume->pixel)[i] = new char;
		(costume->pixel)[i][0] = '0';
	}
	costume->n_col = 1;
	costume->n_row = 5;
	word = "";
}
