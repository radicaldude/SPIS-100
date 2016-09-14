#include "spis.h"

vector<input> inputs;
vector<output> outputs;

void input::loadValue() {
	inArr->nodeSet(INPUT_ID, inList.front());
	inList.pop_front();
	return;
}

void input::inputInt(int input) {
	inList.push_back(input);

	if (inArr->setRequest(INPUT_ID) && inList.size() > 0)
		loadValue();

	return;
}

void input::tickUpdate() {
	if (inList.size() > 0 && inArr->setRequest(INPUT_ID))
		loadValue();

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

void output::tickUpdate() {
	if (outArr->getRequest(OUTPUT_ID))
		takeValue();

	return;
}
