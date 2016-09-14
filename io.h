const int inputListWidth = 4;

class input {
	protected:
		list<int> inList;
		inputArrow *inArr;
	public:
		virtual void loadValue();
		virtual void inputInt(int input);
		virtual void tickUpdate();
};

class output {
	protected:
		list<int> outputList;
		outputArrow *outArr;
	public:
		virtual void takeValue();
		virtual void tickUpdate();
};

class listInput: public input {
	protected:
		string lbl;
		int current;
		WINDOW *win;
		WINDOW *numWin;
		WINDOW *highlight;
	public:
		void loadValue();
		void inputInt(int input);
		void tickUpdate();

		listInput(int startX, int startY, string label, int maxNum, list<int> numbers);
		void initArrow(int8_t nodeIndex, int8_t arrowIndex);
		void updateNumbers(list<int> numbers);
		void drawHighlight(int previous, int previousNum);
};

extern vector<input> inputs;
extern vector<output> outputs;
