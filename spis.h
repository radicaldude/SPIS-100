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
#define updateArrow(a,b) \
  grid[a].arrowUpdate(b)

using namespace std;

const string SIM_OPS[] = { "NOP", "SWP", "SAV", "NEG" };  // Operations with no parameters
const string SRC_OPS[] = { "ADD", "SUB", "JRO" };  // Operations with only SRC as a parameter
const string SD_OPS[] = { "MOV" };  // Operations with both SRC and DST as parameters
const string LAB_OPS[] = { "JMP", "JEZ", "JNZ", "JGZ", "JLZ"}; // Operations with only SRC as a parameter


enum STATUS{
  SET,
  READY,
  WAIT,
  INVALID
};

enum STATE {
	OFF,
	EDIT,
	RUNNING
};

class arrow{
  uint8_t getNodeIndex(int8_t nodeId);

	public:
		int8_t nodeId[2];
		int8_t status[2];
		int16_t value[2];

		WINDOW *win;
		arrow(int8_t nodeId1, int8_t nodeId2);
		bool setRequest(int8_t nodeId);
		bool getRequest(int8_t nodeId);
		int16_t nodeGet(int8_t nodeId);
		void nodeSet(int8_t nodeId, int16_t number);
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
  int backspace(int line, int index);
  std::vector<std::pair<std::string, uint8_t> > labels;
  int getLine(string label);
  arrow *arrows[4];
};

typedef struct{
  arrow *arr;
  std::vector<int> values;
} io;

class input {

};

class output {

};

int get_code(ifstream *file, std::vector<node> &grid);
void compute_tick();
bool is_whitespace(string string);

bool pointInWindow(WINDOW *win, int x, int y);
WINDOW *new_bwin(int height, int width, int starty, int startx);

void initSystem();
void runtimeSystemInput(MEVENT event);
void drawNodeContent();
bool systemInput(int input, MEVENT event);
void drawSystemContent();

extern bool stop;
extern int state;
extern int tickDelay;
extern std::vector<io> inputs, outputs;
extern std::vector<node> grid;
extern void drawHighlight(int i);
//extern void updateArrow(int, int);
extern string makeThreeDigit(int n);
extern bool isNum(string);
extern int x;
extern int y;
