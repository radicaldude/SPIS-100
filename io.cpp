#include "spis.h"

vector<input *> inputs;
vector<output *> outputs;

void input::loadValue() {
	return;
}

void input::inputInt(int input) {
	return;
}

inputArrow *input::initArrow(int8_t nodeIndex, int8_t arrowIndex) {
	return NULL;
}

void input::tickUpdate() {
	if (inArr->setRequest(INPUT_ID))
		loadValue();
	return;
}

void input::reset() {
	return;
}


// output

/*output::output(int8_t id) : outArr(id) {
	return;
}*/

void output::takeValue() {
	outArr->nodeGet(OUTPUT_ID);
	return;
}

outputArrow *output::initArrow(int8_t nodeIndex, int8_t arrowIndex) {
	return NULL;
}

void output::tickUpdate() {
	if (outArr->getRequest(OUTPUT_ID))
		takeValue();

	return;
}

void output::reset() {
	return;
}
