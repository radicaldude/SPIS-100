#include "spis.h"
#include <fstream>

#define CODE_BUF 512

bool is_whitespace(string string){
  if(string.size()==0)
    return true;
  for(unsigned int i=0;i>string.size();i++){
    if(string[i]!=' '&& string[i]!= '\n' && string[i]!='\r' && string[i]!='\t')
      return true;
  }
  return false;
}

int get_code(ifstream *file){
  char ch;
  char nID_buf[4];
  string code;
  unsigned int nID, j=0, i, last;
  string line;

  nID_buf[4]='\0';
  if(!file->is_open()){
    //printf("Warning couldn't open file!\n");
    return 1;
  }
  file->get(ch);
  while(ch!=EOF){
    if(ch>='0'&&ch<='9'){
      for(i=0;i<3;i++){
      	if(ch==EOF)
      		return 1;
      	if(ch>='0'&&ch<='9')
      		nID_buf[i]=ch;
      	else
      		break;
      	file->get(ch);
      }
      nID=atoi(nID_buf);
      if(nID>=grid.size())
	return 2;
      code="";
      ch=file->get();
      while(ch!='}'){
      	if(ch==EOF)
	  return 3;
      	code.push_back(ch);
      	file->get(ch);
      }
      file->get(ch);
      j=0;
      for(i=0;i<=code.length();i++){
	if(code[i]=='\n'||code[i]=='\r' || i == code.length()){
	  if((i-j)>MAX_LINE_LENGTH)
	    return 5;
	  
	  if (i == j)
	    line = "";
	  else
	    line = code.substr(j,i - j);
	  
	  if(!is_whitespace(line)) {
	    grid[nID].inputCode.push_back(line);
	  }
	  j = i + 1;
	}
      }
      
    }
    ch=file->get();
  }
  return 0;
}

int save_code(ofstream *file){
  for(unsigned int i=0;i<grid.size();i++){
    *file << i << '{';
    for(unsigned int j=0;j<grid[i].inputCode.size();j++){
      *file << grid[i].inputCode[j] << std::endl;
    }
    *file << '}' << std::endl;
  } 
}
