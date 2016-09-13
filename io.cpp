#include "spis.h"

// input

input::input(int8_t id) : arr(id) {
	return;
}

void input::loadValue() {
	arr.nodeSet(inputID, inputList.front());
	inputList.pop_front();
	return;
}

void input::inputInt(int input) {
	inputList.push_back(input);

	if (arr.setRequest(inputID) && inputList.size() > 0)
		loadValue();

	return;
}

void input::tickUpdate() {
	if (inputList.size() > 0 && arr.setRequest(inputID))
		loadValue();

	return;
}


// output

output::output(int8_t id) : arr(id) {
	return;
}

void output::takeValue() {
	arr.nodeGet(outputID);
	return;
}

void output::tickUpdate() {
	if (arr.getRequest(outputID))
		takeValue();

	return;
}
