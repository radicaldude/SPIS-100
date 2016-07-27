#include "spis.h"

int compute_tick(std::vector<node> nodes, std::vector<arrow> inputs){
  int i;
  
  for(i=0;i<inputs.size();i++){
    if(inputs[0].set==0)
      
      }
  for(i=0; i<nodes.size();i++){
    std::vector<string> code=nodes[i].code;
    for(int j=0;j<code.size();j++){
      string line = sanitize(code[j]);
      if(strncmp("MOV", line.c_str, 3)){
	int c;
	string src, dest;
	
	operands = line.sub_str(4);
	c=operands.find_first_of(' ');
	src = operands.substr(0,c);
	dest = operands.substr(c);
	if(
	}
      if(strncmp(
    } 
}
