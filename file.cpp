#include "node.h"
#include <iostream>
#include <fstream>
#define CODE_BUF

int get_code(ifstream file, std::vector<node> grid){
  char ch;
  char nID_buf[3];
  char code_buf[CODE_BUF];
  int c=0,node;
  
  if(!file.open())
    return 1;
  file.get(ch);
  while(ch!=EOF){
    for(int i=0;i<3;i++){
      file.get(ch);
      if(ch>='0'&&ch<='9')
	nID_buf[i]=ch;
      else
	break;
    }
    nID=atoi(nID_buf);
    if(!grid[nID])
      return 1;
    while(ch!='{'){
      if(ch==EOF)
	return 1;
      file.get(ch);
    }
    code_buf[0]='/0';
    c=0
      while(ch!='}'){
	if(ch==EOF)
	  return 1;
	if(c==CODE_BUF-1)
	  return 1;
	code_buf[c]=ch;
	c++;
      }
    file.get(ch);
    grid[nID].inputCode=string(code_buf);
  }
  return 0;
}
