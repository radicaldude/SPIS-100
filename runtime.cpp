#include "spis.h"
#include <unistd.h>
std::vector<io> inputs;
std::vector<io> outputs;

void compute_tick(){
  unsigned int i;
  for(i=0;i<inputs.size();i++){
    if(inputs[i].arr->status[0]==SET){
      continue;
    }
    else if(inputs[i].values.size()>0){
      inputs[i].arr->value[0]=inputs[i].values.back();
      inputs[i].values.pop_back();
      inputs[i].arr->status[0]=SET;
    }
  }
  for(i=0;i<grid.size();i++){
    if(stop==TRUE)
      return;
    for(int j=0;j<4;j++){
      if(grid[i].arrows[j]){
      	for(int k=0;k<2;k++)
      		if(grid[i].arrows[j]->status[k]==SET)
      			grid[i].arrows[j]->status[k]==READY;
      }
    }
    if(grid[i].no_code)
      continue;
    if(!grid[i].runline()){
      printf("X %s", grid[i].inputCode[grid[i].pc].c_str());
      state=1;
      flash();
      return;
    }
    if(grid[i].pc>=grid[i].inputCode.size())
      grid[i].pc=0;
    while(is_whitespace(grid[i].inputCode[grid[i].pc])){
      grid[i].pc++;
      if(grid[i].pc>=grid[i].inputCode.size()){
	grid[i].first_instruction();
	break;
      }
    }
  }
  for(i=0;i<outputs.size();i++){
    if(outputs[i].arr->status[0]==SET){
      outputs[i].values.push_back(outputs[i].arr->value[0]);
      outputs[i].arr->status[0]=WAIT;
    }
  }
  return;
}

