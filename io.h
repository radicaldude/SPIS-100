const int INPUT_LIST_WIDTH = 4;

class input {
	protected:
		inputArrow *inArr;
		string lbl;
	public:
		virtual void loadValue();
		virtual void inputInt(int input);
		virtual inputArrow *initArrow(int8_t nodeIndex, int8_t arrowIndex);
		virtual void tickUpdate();
		virtual void reset();
		inputArrow *getArrow() {
			return inArr;
		}
};

class output {
	protected:
		outputArrow *outArr;
		string lbl;
	public:
		virtual void takeValue();
		virtual outputArrow *initArrow(int8_t nodeIndex, int8_t arrowIndex);
		virtual void tickUpdate();
		virtual void reset();
		outputArrow *getArrow() {
			return outArr;
		}
};

class listInput: public input {
	protected:
		int current;
		vector<int> numStorage;
		WINDOW *win;
		WINDOW *numWin;
		WINDOW *highlight;
	public:
		void loadValue();
		void inputInt(int input);
		inputArrow *initArrow(int8_t nodeIndex, int8_t arrowIndex);
		void tickUpdate();
		void reset();

		listInput(int startX, int startY, string label, int maxNum, vector<int> numbers);
		void drawHighlight();
};

class listOutput: public output {
	protected:
		int current;
		vector<int> numStorage;
		vector<int> outputStorage;
		WINDOW *win;
		WINDOW *numWin;
		WINDOW *outWin;
		WINDOW *highlight;
	public:
		void takeValue();
		outputArrow *initArrow(int8_t nodeIndex, int8_t arrowIndex);
		void tickUpdate();
		void reset();

		listOutput(int startX, int startY, string label, int maxNum, vector<int> numbers);
		void drawHighlight();
		void drawFalse();
};

class consoleOutput: public output {
	protected:
		int current;
		int max;
		WINDOW *win;
		WINDOW *outWin;
	public:
		void takeValue();
		outputArrow *initArrow(int8_t nodeIndex, int8_t arrowIndex);
		void tickUpdate();
		void reset();

		consoleOutput(int startX, int startY, string label, int maxNum);
};

class consoleInput: public input, public runtimeInput {
	protected:
		int current;
		int max;
		string inStr;
		bool inReady;
		int inValue;
		int cursorX;
		WINDOW *win;
		WINDOW *numWin;
		WINDOW *inWin;
	public:
		void loadValue();
		void inputInt(int input);
		inputArrow *initArrow(int8_t nodeIndex, int8_t arrowIndex);
		void tickUpdate();
		void reset();

		bool processInput(int input, MEVENT event);
		WINDOW *getInputWin();

		consoleInput(int startX, int startY, string label, int maxNum);
		void updateValues();
};

class visualOutput: public output {
	protected:
		int width, height;
		vector<int> in;
		WINDOW *win;
		WINDOW *visualWin;
		WINDOW *highlight;
	public:
		void takeValue();
		outputArrow *initArrow(int8_t nodeIndex, int8_t arrowIndex);
		void tickUpdate();
		void reset();

		visualOutput(int startX, int startY, string label, int w, int h);
};

extern vector<input *> inputs;
extern vector<output *> outputs;
