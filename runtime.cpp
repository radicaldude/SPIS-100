#include "spis.h"
#include <unistd.h>
std::vector<io> inputs;
std::vector<io> outputs;

void compute_tick(){
  unsigned int i;
  /*for(i=0;i<inputs.size();i++){
    if(inputs[i].arr->status[0]==SET){
      continue;
    }
    else if(inputs[i].values[0]){
      inputs[i].arr->value[0]=inputs[i].values.back();
      inputs[i].values.pop_back();
      inputs[i].arr->status[0]=SET;
    }
  }*/
  for(i=0;i<grid.size();i++){
    if(stop==TRUE)
      return;
    if(!grid[i].runline()){
      printf(("X" + grid[i].inputCode[grid[i].pc]).c_str());
      state=1;
      flash();
      return;
    }
  }
  
  /*for(i=0;i<outputs.size();i++){
    if(outputs[i].arr->status[0]==SET){
      outputs[i].values.push_back(outputs[i].arr->value[0]);
      outputs[i].arr->status[0]=WAIT;
      }
  }*/
  return;
}

void io::get(){
  for(int i=0;i<10;i++){
    //std::cout << "Give me numbers!";
    //this->values.push_back(getch()-'0');
    this->values.push_back(5);
  }
  return;
}

void io::put(){
  for(int i=0;i<this->values.size();i++){
    //std::cout << this->values[i];
  }  
  return;
}
