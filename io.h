#include "spis.h"

class input {
	protected:
		list<int> inputList;
		inputArrow arr;
	public:
		input(int8_t id);
		virtual void loadValue() {
			return;
		}
		virtual void inputInt(int input) {
			return;
		}
		virtual void tickUpdate() {
			return;
		}
};

class output {
	protected:
		list<int> outputList;
		outputArrow arr;
	public:
		output(int8_t);
		virtual void takeValue();
		virtual void tickUpdate();
};

class inputList: public input {
	protected:
		list<int> in;
};

extern vector<input> inputs;
extern vector<output> outputs;
