#include "spis.h"

uint8_t arrowType::getNodeIndex(int8_t id) {
  uint8_t nodeIndex = 0;
  if (id == nodeId[1])
    nodeIndex = 1;
  return nodeIndex;
}


// arrow

arrow::arrow(int8_t nodeId1, int8_t nodeId2, int startY, int startX, bool vertical) {
  nodeId[0] = nodeId1;
  nodeId[1] = nodeId2;

  status[0] = WAIT;
  status[1] = WAIT;

  value[0] = 0;
  value[1] = 0;

  vert = vertical;

  if (vert) {
  	win = win=newwin(ARROW_V_HEIGHT, ARROW_V_WIDTH, startY, startX);
  	wprintw(win, V_ARROW);
  } else {
  	win = win=newwin(ARROW_H_HEIGHT, ARROW_H_WIDTH, startY, startX);
  	wprintw(win, H_ARROW);
  }

	wrefresh(win);
	return;
}

bool arrow::setRequest(int8_t id) {
  uint8_t nodeIndex = getNodeIndex(id);
  if (status[nodeIndex] == WAIT) {
    return true;
  }
  return false;
}

bool arrow::getRequest(int8_t id) {
  uint8_t nodeIndex = getNodeIndex(id);
  if (status[!nodeIndex] == READY) {
    return true;
  }
  return false;
}

int16_t arrow::nodeGet(int8_t id) {
  uint8_t nodeIndex = !getNodeIndex(id);
  int16_t tmpValue = value[nodeIndex];

  status[nodeIndex] = WAIT;
  return tmpValue;
}

void arrow::nodeSet(int8_t id, int16_t number) {
  uint8_t nodeIndex = getNodeIndex(id);
  
  value[nodeIndex] = number;
  status[nodeIndex] = SET;
  return;
}

void arrow::tickUpdate() {
	for(int k=0;k<2;k++)
		if(status[k]==SET)
			status[k]=READY;

	return;
}

void arrow::updateValues() {
	string vals[2];

	for(int i=0;i<2;i++){
		if(status[i] == SET || status[i] == READY)
			vals[i] = makeThreeDigit(value[i]);
		else
			vals[i] = " ?  ";
	}
	if (vert) {
		mvwprintw(win, 0, 0, vals[0].c_str());
		mvwprintw(win, 0, 7, vals[1].c_str());
	} else {
		mvwprintw(win, 0, 0, vals[0].c_str());
		mvwprintw(win, 3, 0, vals[1].c_str());
	}

	wrefresh(win);
}

void arrow::reset() {
  status[0]=WAIT;
  status[1]=WAIT;
}


// inputArrow

inputArrow::inputArrow(int8_t id, int startY, int startX, int arrowIndex, string label) {
	nodeId[0] = id;

	status[0] = WAIT;

	value[0] = 0;

	vert = arrowIndex % 2 == 0;

	if (arrowIndex == 0) {
		win = newwin(ARROW_V_HEIGHT, ARROW_V_WIDTH, startY, startX);
		wprintw(win, DO_ARROW);
	} else if (arrowIndex == 1) {
		win = newwin(ARROW_H_HEIGHT, ARROW_H_WIDTH, startY, startX);
		wprintw(win, LE_ARROW);
	} else if (arrowIndex == 2) {
		win = newwin(ARROW_V_HEIGHT, ARROW_V_WIDTH, startY, startX);
		wprintw(win, UP_ARROW);
	} else {
		win = newwin(ARROW_H_HEIGHT, ARROW_H_WIDTH, startY, startX);
		wprintw(win, RI_ARROW);
	}

	mvwprintw(win, 0, 0, label.c_str());
	//updateValues();

	wrefresh(win);

	return;
}

bool inputArrow::setRequest(int8_t id) {
	if (status[0] == WAIT && id < 0) {
		return true;
	}
	return false;
}

bool inputArrow::getRequest(int8_t id) {
	if (status[0] == READY) {
		return true;
	}
	return false;
}

int16_t inputArrow::nodeGet(int8_t id) {
  status[0] = GOT;
  return value[0];
}

void inputArrow::nodeSet(int8_t id, int16_t number) {
  value[0] = number;
  status[0] = SET;
  return;
}

void inputArrow::tickUpdate() {
	if(status[0]==SET)
		status[0]=READY;
	if (status[0] == GOT)
		status[0] = WAIT;
	return;
}

void inputArrow::updateValues() {
	string val;

	if(status[0] == SET || status[0] == READY)
		val = makeThreeDigit(value[0]);
	else
		val = " ?  ";

	if (vert) {
		mvwprintw(win, 0, 7, val.c_str());
	} else {
		mvwprintw(win, 3, 0, val.c_str());
	}

	wrefresh(win);
}

void inputArrow::reset() {
  status[0]=WAIT;
  status[1]=WAIT;
}


// outputArrow

outputArrow::outputArrow(int8_t id, int startY, int startX, int arrowIndex, string label) {
	nodeId[0] = id;

	status[0] = WAIT;

	value[0] = 0;

	vert = arrowIndex % 2 == 0;

	if (arrowIndex == 0) {
		win = newwin(ARROW_V_HEIGHT, ARROW_V_WIDTH, startY, startX);
		wprintw(win, UP_ARROW);
	} else if (arrowIndex == 1) {
		win = newwin(ARROW_H_HEIGHT, ARROW_H_WIDTH, startY, startX);
		wprintw(win, RI_ARROW);
	} else if (arrowIndex == 2) {
		win = newwin(ARROW_V_HEIGHT, ARROW_V_WIDTH, startY, startX);
		wprintw(win, DO_ARROW);
	} else {
		win = newwin(ARROW_H_HEIGHT, ARROW_H_WIDTH, startY, startX);
		wprintw(win, LE_ARROW);
	}

	mvwprintw(win, 0, 0, label.c_str());

	wrefresh(win);

	return;
}

bool outputArrow::setRequest(int8_t id) {
	if (status[0] == WAIT) {
		return true;
	}
	return false;
}

bool outputArrow::getRequest(int8_t id) {
	if (status[0] == READY && id < 0) {
		return true;
	}
	return false;
}

int16_t outputArrow::nodeGet(int8_t id) {
  status[0] = GOT;
  return value[0];
}

void outputArrow::nodeSet(int8_t id, int16_t number) {
  value[0] = number;
  status[0] = SET;
  return;
}

void outputArrow::tickUpdate() {
	if(status[0]==SET)
		status[0]=READY;
	if (status[0] == GOT)
		status[0] = WAIT;
	return;
}

void outputArrow::updateValues() {
	string val;

	if(status[0] == SET || status[0] == READY)
		val = makeThreeDigit(value[0]);
	else
		val = " ?  ";

	if (vert) {
		mvwprintw(win, 0, 7, val.c_str());
	} else {
		mvwprintw(win, 3, 0, val.c_str());
	}

	wrefresh(win);
}

void outputArrow::reset() {
  status[0]=WAIT;
  status[1]=WAIT;
}
