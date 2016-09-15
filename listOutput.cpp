#include "spis.h"

listOutput::listOutput(int startX, int startY, string label, int maxNum, vector<int> numbers) {
	win = newwin(maxNum + 1 + 2, inputListWidth + 2, startY, startX);
	mvwprintw(win, 0, 0, label.c_str());
	wrefresh(win);
	outWin = new_bwin(maxNum + 2, inputListWidth + 2, startY + 1, startX + 5);
	numWin = new_bwin(maxNum + 2, inputListWidth + 2, startY + 1, startX);

	numStorage = numbers;
	current = 0;

	lbl = label;

	for (int i = 0; i < numStorage.size(); i++) {
		mvwprintw(numWin, i + 1, 1, makeThreeDigit(numStorage[i]).c_str());
	}

	wrefresh(numWin);
}

outputArrow *listOutput::initArrow(int8_t nodeIndex, int8_t arrowIndex) {
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

void listOutput::takeValue() {
	int val = outArr->nodeGet(OUTPUT_ID);
	mvwprintw(outWin, current + 1, 1, makeThreeDigit(val).c_str());
	wrefresh(outWin);

	if (val != numStorage[current])
		drawFalse();

	return;
}

void listOutput::tickUpdate() {
	if (outArr->getRequest(INPUT_ID)) {
		takeValue();
		current++;
		drawHighlight();
	}

	return;
}
void listOutput::reset() {
	current = 0;
}

void listOutput::drawHighlight() {

}

void listOutput::drawFalse() {
	flash();
}
