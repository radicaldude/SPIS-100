#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <stdint.h>
#include <ncurses.h>
#include <string.h>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <list>

#define MAX_LINE_LENGTH 17
#define NODE_HEIGHT 11
#define ARROW_H_WIDTH 4
#define ARROW_H_HEIGHT 4
#define ARROW_V_WIDTH 12
#define ARROW_V_HEIGHT 1
#define GAP_WIDTH_H 1
#define GAP_WIDTH_V 0
#define NODE_WIDTH 26
#define CODE_WIDTH 20
#define BUTTON_WIDTH 5
#define BUTTON_HEIGHT 3

#ifdef _ascii_only
#define H_ARROW "\n---><---"
#define V_ARROW "    v^"
#else
#define V_ARROW "   ⇧⇩"
#define H_ARROW "\n ⇨\n ⇦"
#endif

using namespace std;

const string SIM_OPS[] = { "NOP", "SWP", "SAV", "NEG" };  // Operations with no parameters
const string SRC_OPS[] = { "ADD", "SUB", "JRO" };  // Operations with only SRC as a parameter
const string SD_OPS[] = { "MOV" };  // Operations with both SRC and DST as parameters
const string LAB_OPS[] = { "JMP", "JEZ", "JNZ", "JGZ", "JLZ"}; // Operations with only SRC as a parameter

const int INPUT_ID = -1;
const int OUTPUT_ID = -2;

enum STATUS{
  SET,
  READY,
  WAIT,
  INVALID,
	GOT
};

enum STATE {
	OFF,
	EDIT,
	RUNNING
};

class arrowType{
	protected:
  	int8_t nodeId[2];
  	bool vert;

	public:
		int8_t status[2];
		int16_t value[2];
		uint8_t getNodeIndex(int8_t id);
		virtual bool setRequest(int8_t id) {
			return false;
		}
		virtual bool getRequest(int8_t id) {
			return false;
		}
		virtual int16_t nodeGet(int8_t id) {
			return 0;
		}
		virtual void nodeSet(int8_t id, int16_t number) {
			return;
		}
		virtual void tickUpdate() {
			return;
		}
		virtual void updateValues() {
			return;
		}
		virtual void reset() {
			return;
		}

		WINDOW *win;
};

class arrow: public arrowType {
	public:
		arrow(int8_t nodeId1, int8_t nodeId2, int startX, int startY, bool vertical);
		bool setRequest(int8_t id);
		bool getRequest(int8_t id);
		int16_t nodeGet(int8_t id);
		void nodeSet(int8_t id, int16_t number);
		void tickUpdate();
		void updateValues();
		void reset();
};

class node{
 private:
  uint8_t nodeId;
  pair<int8_t, int16_t> getFromSrc(string src);
 public:
  WINDOW *w_main;
  WINDOW *w_code;
  WINDOW *w_reg;
  WINDOW *w_highlight;
  node(uint8_t nId);
  int16_t acc;
  int16_t bak;
  int8_t  pc;
  bool no_code;
  std::vector<string> inputCode;
  std::vector<string> code;
  bool runline();
  bool runPrepare();
  void reset();
  void first_instruction();
  void arrowUpdate(unsigned int);
  void inputChar(int line, int index, char ch);
  void newLine(int line, int index);
  int  backspace(int line, int index);
  std::list<std::pair<std::string, uint8_t> > labels;
  int getLine(string label);
  arrowType *arrows[4];
};

class inputArrow: public arrowType {
	public:
		inputArrow(int8_t id, int startY, int startX, bool vertical, string label);
		bool setRequest(int8_t id);
		bool getRequest(int8_t id);
		int16_t nodeGet(int8_t id);
		void nodeSet(int8_t id, int16_t number);
		void tickUpdate();
		void updateValues();
		void reset();
};

class outputArrow: public arrowType {
	public:
		outputArrow(int8_t id);
		bool setRequest(int8_t id);
		bool getRequest(int8_t id);
		int16_t nodeGet(int8_t id);
		void nodeSet(int8_t id, int16_t number);
		void tickUpdate();
		void updateValues();
		void reset();
};

int get_code(ifstream *file, std::vector<node> &grid);
void compute_tick();
bool is_whitespace(string string);

bool pointInWindow(WINDOW *win, int x, int y);
WINDOW *new_bwin(int height, int width, int starty, int startx);

void initSystem(int begX, int begY);
void runtimeSystemInput(MEVENT event);
void drawNodeContent();
bool systemInput(int input, MEVENT event);
void drawSystemContent();

extern bool stop;
extern int state;
extern int tickDelay;
extern std::vector<node> grid;
extern vector<arrowType *> gridArrows;
extern void drawHighlight(int i);
extern string makeThreeDigit(int n);
extern bool isNum(string);
extern int x;
extern int y;

#include "io.h"
