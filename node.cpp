#include "spis.h"

class arrow{
public:
  int8_t status[2];
  int16_t value[2];
};
  
class node{
public:
  node(uint8_t arrows);
  uint8_t pc;
  uint8_t arrow_id;
  int16_t acc;
  int16_t bak;
  std::vector<arrow> *arrow;
  std::vector<string> code;
};

void node::node(int arrows){
  acc=0;
  bak=0;
  pc=0;
  node = malloc(arrows*sizeof(&this));  
}


