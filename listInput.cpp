#include "spis.h"

listInput::listInput(int startX, int startY, string label, int maxNum, list<int> numbers) {
	win = newwin(maxNum + 1 + 2, inputListWidth + 2, startY, startX);
	mvwprintw(win, 0, 0, label.c_str());
	wrefresh(win);
	numWin = new_bwin(maxNum + 2, inputListWidth + 2, startY + 1, startX);

	inList = numbers;
	lbl = label;

	int i = 0;
	for (list<int>::iterator it = inList.begin(); it != inList.end(); ++it) {
		mvwprintw(numWin, i + 1, 1, makeThreeDigit(*it).c_str());
		i++;
	}

	wrefresh(numWin);

}

void listInput::initArrow(int8_t nodeIndex, int8_t arrowIndex) {
	int y, x;
	getbegyx(grid[nodeIndex].w_main, y, x);

	if (arrowIndex == 0)
		inArr = new inputArrow(nodeIndex, y - ARROW_V_HEIGHT, x + floor(NODE_WIDTH / 2) - floor(ARROW_V_WIDTH / 2), true, lbl);
	else if (arrowIndex == 1)
		inArr = new inputArrow(nodeIndex, y + floor(NODE_HEIGHT / 2) - floor(ARROW_H_HEIGHT / 2), x + NODE_WIDTH + GAP_WIDTH_H, false, lbl);
	else if (arrowIndex == 2)
		inArr = new inputArrow(nodeIndex, y + NODE_HEIGHT + GAP_WIDTH_V, x + floor(NODE_WIDTH / 2) - floor(ARROW_V_WIDTH / 2), true, lbl);
	else
		inArr = new inputArrow(nodeIndex, y + floor(NODE_HEIGHT / 2) - floor(ARROW_H_HEIGHT / 2), x - ARROW_H_WIDTH, false, lbl);

	gridArrows.push_back(inArr);
	grid[nodeIndex].arrows[arrowIndex] = inArr;

	wrefresh(inArr->win);
}

void listInput::loadValue() {
	inArr->nodeSet(INPUT_ID, inList.front());
	return;
}

void listInput::inputInt(int input) {
	return;
}

void listInput::tickUpdate() {
	if (inList.size() > 0 && inArr->setRequest(INPUT_ID))
		loadValue();

	return;
}

void listInput::drawHighlight(int previous, int previousNum) {
	highlight = newwin(1, inputListWidth, getbegy(numWin) + current, getbegx(numWin));
	wbkgd(highlight, COLOR_PAIR(1));
	wbkgd(numWin, COLOR_PAIR(5));
	wprintw(highlight, makeThreeDigit(inList.front()).c_str());
}
