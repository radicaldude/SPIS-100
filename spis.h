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

#define MAX_LINE_LENGTH 17


using namespace std;

const string SIM_OPS[] = { "NOP", "SWP", "SAV", "NEG" };  // Operations with no parameters
const string SRC_OPS[] = { "ADD", "SUB", "JRO" };  // Operations with only SRC as a parameter
const string SD_OPS[] = { "MOV" };  // Operations with both SRC and DST as parameters
const string LAB_OPS[] = { "JMP", "JEZ", "JNZ", "JGZ", "JLZ"}; // Operations with only SRC as a parameter


enum STATUS{
  SET,
  WAIT,
  INVALID
};

class arrow{
  uint8_t getNodeIndex(int8_t nodeId);

public:
  int8_t nodeId[2];
  int8_t status[2];
  int16_t value[2];

  WINDOW *win;
  arrow(int8_t nodeId1, int8_t nodeId2);
  bool nodeRequest(int8_t nodeId);
  int16_t nodeGet(int8_t nodeId);
  void nodeSet(int8_t nodeId, int16_t number);
};

class io{
 public:
  arrow *arr; 
  std::vector<int> values;
  void get();
  void put();
};

class node{
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
  std::vector<string> code;
  std::vector<string> inputCode;
  bool runline();
  bool runPrepare();
  void inputChar(int line, int index, char ch);
  void newLine(int line, int index);
  int backspace(int line, int index);
  std::map<uint8_t, uint16_t> labels;
  arrow *arrows[4];
};

int get_code(ifstream *file, std::vector<node> &grid);
void compute_tick();
bool is_whitespace(string string);
extern bool stop;
extern int state;
extern std::vector<io> inputs, outputs;
extern std::vector<node> grid;
