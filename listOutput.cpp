#include "spis.h"

listOutput::listOutput(int startX, int startY, string label, int maxNum, vector<int> numbers) {
	win = newwin(maxNum + 1 + 2, INPUT_LIST_WIDTH + 2, startY, startX);
	mvwprintw(win, 0, 0, label.c_str());
	wrefresh(win);
	outWin = winBorder(maxNum, INPUT_LIST_WIDTH, startY + 2, startX + 6);
	numWin = winBorder(maxNum, INPUT_LIST_WIDTH, startY + 2, startX + 1);

	numStorage = numbers;
	current = 0;

	lbl = label;

	for (int i = 0; i < numStorage.size(); i++) {
		mvwprintw(numWin, i, 0, makeThreeDigit(numStorage[i]).c_str());
	}

	wrefresh(numWin);
	highlight=NULL;
	return;
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
	if(highlight)
	  werase(highlight);
}

void listOutput::drawHighlight() {
  getbegyx(numWin, y, x);
  if(highlight){
    werase(highlight);
    delwin(highlight);
  }
  highlight = newwin(1, INPUT_LIST_WIDTH,y + current, x + 1);
  wbkgd(numWin, COLOR_PAIR(5));
  wbkgd(highlight, COLOR_PAIR(1));
  wprintw(highlight, makeThreeDigit(numStorage[current+1]).c_str());
  wrefresh(highlight);
}

void listOutput::drawFalse() {
	flash();
}
