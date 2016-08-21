#include "spis.h"

#define CODE_BUF 128

int get_code(ifstream *file, std::vector<node> grid){
  char ch;
  char nID_buf[3];
  char code_buf[CODE_BUF];
  int c=0,nID, j=0;
  
  if(!file->is_open())
    return 1;
  file->get(ch);
  while(ch!=EOF){
    if(ch>='0'&&ch<='9'){
      for(int i=0;i<3;i++){
	file->get(ch);
	if(ch==EOF)
	  return 1;
	if(ch>='0'&&ch<='9')
	  nID_buf[i]=ch;
	else
	  break;
      }
      nID=atoi(nID_buf);
    if(nID>=grid.size())
      return 2;
    while(ch!='{'){
    }
    code_buf[0]='\0';
    c=0;
    ch=file->get();
    while(ch!='}'){
      if(ch==EOF)
	return 3;
      if(c==CODE_BUF-1)
	return 4;
      code_buf[c]=ch;
      c++;
      file->get(ch);
    }
    file->get(ch);
    j=0;
    for(int i=0;i<c;i++){
      char line_buf[MAX_LINE_LENGTH];
      if(code_buf[i]=='\n'||code_buf[i]=='\r'){
	strncpy(line_buf,code_buf+j,i-j);
	line_buf[i-j]='\0';
	grid[nID].inputCode.push_back(std::string(line_buf));
	j=i;
      }
    }
    }
    ch=file->get();
  }
  return 0;
}
