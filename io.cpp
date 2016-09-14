#include "spis.h"

vector<input> inputs;
vector<output> outputs;

input::input(int8_t id) : arr(id) {
	return;
}

void input::loadValue() {
	arr.nodeSet(INPUT_ID, inputList.front());
	inputList.pop_front();
	return;
}

void input::inputInt(int input) {
	inputList.push_back(input);

	if (arr.setRequest(INPUT_ID) && inputList.size() > 0)
		loadValue();

	return;
}

void input::tickUpdate() {
	if (inputList.size() > 0 && arr.setRequest(INPUT_ID))
		loadValue();

	return;
}


// output

output::output(int8_t id) : arr(id) {
	return;
}

void output::takeValue() {
	arr.nodeGet(OUTPUT_ID);
	return;
}

void output::tickUpdate() {
	if (arr.getRequest(OUTPUT_ID))
		takeValue();

	return;
}
