#include "spis.h"

visualOutput::visualOutput(int startX, int startY, string label, int w, int h) {
	width = w;
	height = h;
	win = newwin(height + 3, width + 2, startY, startX);
	mvwprintw(win, 0, abs(float(width / 2 - label.length())), label.c_str());
	wrefresh(win);
	visualWin = winBorder(height, width, startY + 2, startX + 1);
	wrefresh(visualWin);

	lbl = label;
}

outputArrow *visualOutput::initArrow(int8_t nodeIndex, int8_t arrowIndex) {
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

void visualOutput::takeValue() {
	int val = outArr->nodeGet(OUTPUT_ID);
	in.push_back(val);

	if (val == -1)
		in.clear();

	if (in.size() > 2 && in[0] + in.size() - 3 < width) {
		flash();
		int y,x;
		getbegyx(visualWin, y, x);
		highlight = newwin(1, 1, in[1] + y, in[0] + in.size() - 3 + x);
		wbkgd(highlight, COLOR_PAIR(val + 6));
		mvwprintw(highlight, y, x + in.size() - 3, " ");
		wrefresh(highlight);
		delwin(highlight);
	}

	return;
}

void visualOutput::tickUpdate() {
	if (outArr->getRequest(INPUT_ID)) {
		takeValue();
	}

	return;
}
void visualOutput::reset() {
	in.clear();
	werase(visualWin);
}
