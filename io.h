const int inputListWidth = 4;

class input {
	protected:
		list<int> inputList;
		inputArrow arr;
	public:
		input(int8_t id);
		virtual void loadValue();
		virtual void inputInt(int input);
		virtual void tickUpdate();
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

class listInput: public input {
	protected:
		string label;
		int current;
		WINDOW *win;
		WINDOW *numWin;
		WINDOW *highlight;
	public:
		void loadValue();
		void inputInt(int input);
		void tickUpdate();

		listInput(int startX, int startY, int maxNum, list<int> numbers, int8_t arrowNode, string label);
		void updateNumbers(list<int> numbers);
		void drawHighlight(int previous, int previousNum);
};

extern vector<input> inputs;
extern vector<output> outputs;
