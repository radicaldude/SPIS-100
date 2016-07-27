#include "spis.h"

class arrow{
  int8_t set[2];
  int16_t value[2];
};
  
class node{
public:
  node(uint8_t arrows);
  uint8_t pc;
  int16_t acc;
  int16_t bak;
  
  std::vector<arrow> *arrow;
  std::vector<node>   **neighbours;
  std::vector<string> code;
};

void node::node(int arrows){
  acc=0;
  bak=0;
  pc=0;
  node = malloc(arrows*sizeof(&this));  
}


