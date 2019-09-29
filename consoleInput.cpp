#include "spis.h"

consoleInput::consoleInput(int startX, int startY, string label, int maxNum) {
	win = newwin(maxNum + 1 + 2 + 2, INPUT_LIST_WIDTH + 3, startY, startX);
	mvwprintw(win, 0, 0, label.c_str());
	wrefresh(win);
	numWin = winBorder(maxNum , INPUT_LIST_WIDTH + 1, startY + 2, startX + 1);
	wrefresh(numWin);
	inWin = winBorder(1, INPUT_LIST_WIDTH + 1, startY + maxNum + 2 + 1, startX + 1);

	current = 0;
	max = maxNum;
	lbl = label;

	mvwprintw(inWin, 0, 0, PROMPT.c_str());
	wrefresh(inWin);
}

inputArrow *consoleInput::initArrow(int8_t nodeIndex, int8_t arrowIndex) {
	int y, x;
	getbegyx(grid[nodeIndex].w_main, y, x);

	if (arrowIndex == 0)
		inArr = new inputArrow(nodeIndex, y - ARROW_V_HEIGHT, x + floor(NODE_WIDTH / 2) - floor(ARROW_V_WIDTH / 2), arrowIndex, lbl);
	else if (arrowIndex == 1)
		inArr = new inputArrow(nodeIndex, y + floor(NODE_HEIGHT / 2) - floor(ARROW_H_HEIGHT / 2), x + NODE_WIDTH + GAP_WIDTH_H, arrowIndex, lbl);
	else if (arrowIndex == 2)
		inArr = new inputArrow(nodeIndex, y + NODE_HEIGHT + GAP_WIDTH_V, x + floor(NODE_WIDTH / 2) - floor(ARROW_V_WIDTH / 2), arrowIndex, lbl);
	else
		inArr = new inputArrow(nodeIndex, y + floor(NODE_HEIGHT / 2) - floor(ARROW_H_HEIGHT / 2), x - ARROW_H_WIDTH, arrowIndex, lbl);

	gridArrows.push_back(inArr);
	grid[nodeIndex].arrows[arrowIndex] = inArr;

	wrefresh(inArr->win);

	return inArr;
}

void consoleInput::loadValue() {
	inArr->nodeSet(INPUT_ID, inValue);
	inArr->updateValues();
	return;
}

void consoleInput::inputInt(int input) {
	return;
}

void consoleInput::tickUpdate() {
	if (inReady && inArr->setRequest(INPUT_ID)) {
		loadValue();
		inReady = false;
	}
	return;
}

void consoleInput::reset() {
	current = 0;
	werase(numWin);
	wrefresh(numWin);

	selected = false;

	inStr = "";
	updateValues();
}

bool consoleInput::processInput(int input, MEVENT event) {
	if ((input != KEY_MOUSE) && !selected)
		return false;

	if (input == KEY_MOUSE) {

		if (!pointInWindow(inWin, event.x, event.y))
			return false;

		selected = true;
		cursorX = event.x - getbegx(inWin) - 1;
		if (cursorX < 0)
			cursorX = 0;
		else if (cursorX > inStr.length())
			cursorX = inStr.length();

		x = getbegx(inWin) + cursorX + 1;
		y = getbegy(inWin);

		move(y, x);
		setCursor(true);
		return true;
	}

	if (input >= 48 && input <= 57) {
		if (inStr.length() >= 4)
			return true;

		inStr = inStr.substr(0, cursorX) + static_cast<char>(input) + inStr.substr(cursorX);
		updateValues();
		cursorX++;
		x++;
		move(y, x);
		setCursor(true);
	} else if (input == KEY_ENTER || input == 11 || input == 10) {
		if (inArr->setRequest(INPUT_ID) && !inReady && abs(stoi(inStr)) < 999) {
			mvwprintw(numWin, current, 0, makeThreeDigit(stoi(inStr)).c_str());
			wrefresh(numWin);
			inReady = true;
			inValue = stoi(inStr);
		}

		inStr = "";
		x -= cursorX;
		cursorX = 0;
		updateValues();
	} else if(input == KEY_BACKSPACE || input == 127) {
		// BACKSPACE
		if (cursorX <= 0)
			return true;

		inStr = inStr.substr(0, cursorX - 1) + inStr.substr(cursorX);
		cursorX--;
		x = getbegx(inWin) + cursorX + 1;
		move(y, x);
		updateValues();
		setCursor(true);
	} else if (input == KEY_LEFT) {
		if (cursorX != 0) {
			cursorX--;
			x = getbegx(inWin) + cursorX + 1;

			move(y, x);
			setCursor(true);
		}
		return true;
	} else if (input == KEY_RIGHT) {
		if (cursorX <= 4 && cursorX <= inStr.length()) {
			cursorX++;
			x = getbegx(inWin) + cursorX + 1;

			move(y, x);
			setCursor(true);
		}
		return true;
	}
	return false;
}

WINDOW *consoleInput::getInputWin() {
	return inWin;
}

void consoleInput::updateValues() {
	werase(inWin);
	mvwprintw(inWin, 0, 0, (">" + inStr).c_str());
	wrefresh(inWin);
}
