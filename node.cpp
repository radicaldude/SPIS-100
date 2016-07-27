#include "spis.h"

typedef struct{
  bool set;
  int8_t value;
} output;
  
class node{
public:
  node(int arrows);
  uint8_t pc;
  int8_t acc;
  int8_t bak;
  struct out *out;
  std::vector<string> code;
};

void node::node(int arrows){
  acc=0;
  bak=0;
  pc=0;
  out = malloc(arrows*sizeof(output));
  node = malloc(arrows*sizeof(node));  
}
