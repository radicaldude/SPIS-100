#include "spis.h"
//#include "io.h"

listInput::listInput(int startX, int startY, int maxNum, list<int> numbers, int8_t arrowNode, string label) : input(arrowNode) {
	//win = newwin(maxNum + 1 + 2, inputListWidth + 2, startY, startX);
	//wprintw(win, label.c_str());
	numWin = new_bwin(maxNum + 2, inputListWidth + 2, startY + 1 + 1, startX + 1);

	inputList = numbers;

	int i = 0;
	for (list<int>::iterator it = inputList.begin(); it != inputList.end(); ++it) {
		mvwprintw(numWin, i + 1, 1, makeThreeDigit(*it).c_str());
		i++;
	}

	wrefresh(numWin);
}

void listInput::loadValue() {
	arr.nodeSet(INPUT_ID, inputList.front());
	return;
}

void listInput::inputInt(int input) {
	return;
}

void listInput::tickUpdate() {
	if (inputList.size() > 0 && arr.setRequest(INPUT_ID))
		loadValue();

	return;
}

void listInput::drawHighlight(int previous, int previousNum) {
	highlight = newwin(1, inputListWidth, getbegy(numWin) + current, getbegx(numWin));
	wbkgd(highlight, COLOR_PAIR(1));
	wbkgd(numWin, COLOR_PAIR(5));
	wprintw(highlight, makeThreeDigit(inputList.front()).c_str());
}
