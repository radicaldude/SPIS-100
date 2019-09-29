#include "spis.h"
#include <unistd.h>

void compute_tick(){
  unsigned int i;

  for (i = 0; i < gridArrows.size(); i++) {
    gridArrows[i]->tickUpdate();
  }

  for(i=0;i<grid.size();i++){
    if(stop==TRUE)
      return;
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

  for (i=0; i < inputs.size(); i++) {
    inputs[i]->tickUpdate();
  }

  for (i=0; i < outputs.size(); i++) {
    outputs[i]->tickUpdate();
  }

  return;
}

