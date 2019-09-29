#include "spis.h"

consoleOutput::consoleOutput(int startX, int startY, string label, int maxNum) {
	win = newwin(maxNum + 1 + 2, INPUT_LIST_WIDTH + 2, startY, startX);
	mvwprintw(win, 0, 0, label.c_str());
	wrefresh(win);
	outWin = new_bwin(maxNum + 2, INPUT_LIST_WIDTH + 2, startY + 1, startX);

	current = 0;
	max = maxNum;

	lbl = label;
}

outputArrow *consoleOutput::initArrow(int8_t nodeIndex, int8_t arrowIndex) {
	int y, x;
	getbegyx(grid[nodeIndex].w_main, y, x);

	if (arrowIndex == 0)
		outArr = new outputArrow(nodeIndex, y - ARROW_V_HEIGHT, x + floor(NODE_WIDTH / 2) - floor(ARROW_V_WIDTH / 2), arrowIndex, lbl);
	else if (arrowIndex == 1)
		outArr = new outputArrow(nodeIndex, y + floor(NODE_HEIGHT / 2) - floor(ARROW_H_HEIGHT / 2), x + NODE_WIDTH + GAP_WIDTH_H, arrowIndex, lbl);
	else if (arrowIndex == 2)
		outArr = new outputArrow(nodeIndex, y + NODE_HEIGHT + GAP_WIDTH_V, x + floor(NODE_WIDTH / 2) - floor(ARROW_V_WIDTH / 2), arrowIndex, lbl);
	else
		outArr = new outputArrow(nodeIndex, y + floor(NODE_HEIGHT / 2) - floor(ARROW_H_HEIGHT / 2), x - ARROW_H_WIDTH, arrowIndex, lbl);

	gridArrows.push_back(outArr);
	grid[nodeIndex].arrows[arrowIndex] = outArr;

	wrefresh(outArr->win);

	return outArr;
}

void consoleOutput::takeValue() {
	int val = outArr->nodeGet(OUTPUT_ID);
	mvwprintw(outWin, current + 1, 1, makeThreeDigit(val).c_str());
	wrefresh(outWin);

	return;
}

void consoleOutput::tickUpdate() {
	if (outArr->getRequest(INPUT_ID) && current <= max) {
		takeValue();
		current++;
	}

	return;
}
void consoleOutput::reset() {
	current = 0;
	werase(outWin);
}
