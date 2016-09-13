#include "spis.h"

void input::loadValue() {
	if (arr.setRequest(-1) && inputList.size() > 0) {
		arr.nodeSet(-1, inputList.front());
		inputList.pop_front();
	}
}

void input::inputInt(int input) {
	inputList.push_back(input);
	loadValue();
}


