#include "spis.h"

node::node(uint8_t nId){
  nodeId = nId;
  acc = 0;
  bak = 0;
  pc=0;
  no_code=FALSE;

  for (int i = 0; i < 4; i++)
  	arrows[i] = NULL;
}

bool node::runline(){
  //string line = sanitize(code[this.pc]);
  if(code.size()==0)
    return true;
  if(is_whitespace(code[pc]) || code[pc].length() == 0){
  	pc++;
  	if(pc>=code.size()-1)
  	    pc=0;
    return true;
  }
  string line = code[pc];
  if(!strncmp("NOP", line.c_str(), 3)){
    pc++;
    return true;
  } else if(!strncmp("MOV", line.c_str(), 3)){
    int c;
    std::string src, dst;
    pair<int8_t, uint16_t> p;
    int16_t input;
    arrowType *a;

    string operands = line.substr(4);
    c = operands.find_first_of(' ');
    src = operands.substr(0,c);
    dst = operands.substr(c + 1);
    p = getFromSrc(src);
  
    if(!p.first){
      return true;
    }
    input = p.second;
    if(!strncmp(dst.c_str(), "ACC", 3)) {
      acc = input;
    } else if(!strncmp(dst.c_str(), "NIL",3)) {
	
    }else{
      unsigned int a_num;
      if(!strncmp("LEF", dst.c_str(), 3)){
	a_num=3;
      } else if(!strncmp(dst.c_str(), "RIG", 3)) {
	a_num=1;;
      } else if(!strncmp(dst.c_str(), "UP",2)) {
	a_num=0;
      } else if(!strncmp(dst.c_str(),"DOW", 3)) {
    	a_num=2;
      }
      else
	return false;
      a=arrows[a_num];
      if(!a)
	return false;
      if (a->setRequest(nodeId)){
	a->nodeSet(nodeId, input);
	a->updateValues();
      }
      else
	pc--;
    }
    pc++;
    return true;
  }
    else if(!strncmp("SWP", line.c_str(), 3)){
    int16_t tmpAcc = acc;
    acc = bak;
    bak = tmpAcc;
    pc++;
    return true;
    
  } else if(!strncmp("SAV", line.c_str(), 3)){
    bak = acc;
    pc++;
    return true;

  } else if(!strncmp("ADD", line.c_str(), 3)){
    string src = line.substr(4);
    pair<bool, int16_t> p = getFromSrc(src);
    acc += p.second;
    pc++;
    return true;
    } else if(!strncmp("SUB", line.c_str(), 3)){
    string src = line.substr(4);
    pair<int8_t, int16_t> p = getFromSrc(src);
    
    int16_t input;
 
    if (!p.first) {
      return true;
    }
   
    else{
      input = p.second;
      acc -= input;
      pc++;
      return true;
    }
    
  } else if(!strncmp("NEG", line.c_str(), 3)){
    acc = -acc;
    pc++;
    return true;   
    }
  else if(!strncmp("J",line.c_str(),1)){
    unsigned int p;
    if(!strncmp("JRO", line.c_str(), 3)){
      string operands = line.substr(4);
      string src = operands.substr(0);
      pair<int8_t, int16_t> p = getFromSrc(src);
      if(!p.first)
	 return true;
      else
	pc=pc+p.second;
      return true;
    }
    else{
      p=getLine(line.substr(4));
      if(p<0)
	return false;
      if(strncmp("JGZ", line.c_str(),3) && strncmp("JLZ", line.c_str(),3)
	 && strncmp("JEZ", line.c_str(),3) && strncmp("JNZ", line.c_str(),3)&& strncmp("JEZ", line.c_str(), 3) && strncmp("JMP", line.c_str(),3))
	return false;
      
      else if(!strncmp("JGZ", line.c_str(),3)&&this->acc>0)
	pc=p;
      else if(!strncmp("JLZ", line.c_str(),3) &&this->acc<0)
	pc=p;
      else if(!strncmp("JEZ", line.c_str(),3)&&this->acc==0)
	pc=p;
      else if(!strncmp("JNZ", line.c_str(),3)&&this->acc!=0)
	pc=p;
      else if(!strncmp("JMP", line.c_str(),3))
	pc=p;
      else
	pc++;
      return true;
    }
  }
  else
    return false;
}
              
bool node::runPrepare(){
  std::vector<int> labelLines;
  labels.clear();
  code.resize(inputCode.size());
  for (int8_t i = 0; i < inputCode.size(); i++){
    bool label=false;
    for(unsigned int j=0;j<inputCode.size();j++){
      string line=inputCode[i];
      char ch=line[j];
      string tmp;      
      if(ch==':'){
	std::pair<std::string, uint8_t> p;
	int c;
	p.first=line.substr(0,j);
	p.second=i;
	labels.push_back(p);
	label=true;
	c=line.substr(j+1).find_first_not_of(" ");
	code[i]=line.substr(c+j+1);
	break;
      }
    }
    if(label==false){
      code[i]=inputCode[i];
    }
  }
  for(int i=0; i<labelLines.size();i++){
    string operand;
    bool label_exists=false;
    operand=code[labelLines[i]].substr(4);
    for(list<pair<string, uint8_t> >::iterator j=labels.begin();j!=labels.end();j++){
      if(j->first==operand){
	label_exists=true;
	break;
      }
    }
    if(label_exists==false)
      return false;
  }
  //TODO remove extra whitespace
}

void node::inputChar(int line, int index, char ch) {
  if (inputCode.size() > line) {
    if (inputCode[line].length() < MAX_LINE_LENGTH) {
      inputCode[line] = inputCode[line].substr(0, index) + ch + inputCode[line].substr(index);
      //inputCode[line] += ch;
    }
  } else {
    inputCode[line] = ch;
  }
}

void node::newLine(int line, int index) {
  inputCode.insert(inputCode.begin() + line, inputCode[line].substr(0, index));
  inputCode[line + 1] = inputCode[line + 1].substr(index);
}

int node::backspace(int line, int index) {
  if (index > 0) {
    if (inputCode[line].length() > index) {
      inputCode[line] = inputCode[line].substr(0, index - 1) + inputCode[line].substr(index);
    } else if (inputCode[line].length() == index) {
      inputCode[line] = inputCode[line].substr(0, index - 1);
    }
    return 1;
  } else if(line > 0 && inputCode[line - 1].length() + inputCode[line].length() <= MAX_LINE_LENGTH && index == 0) {
    inputCode[line - 1] += inputCode[line];
    inputCode.erase(inputCode.begin() + line);
    return 2;
  }
  
  return 0;
}

pair<int8_t, int16_t> node::getFromSrc(string src) {
  if(src.find_first_not_of("0123456789") == std::string::npos) {
    // Then src is just a number
    pair<bool, int16_t> p;
    p.first = READY;
    try{
      p.second = atoi(src.c_str());
    }
    catch(...){
      p.first = INVALID;
      return p;
    }
    return p;
  } else {
    if (src == "ACC") {
      pair<bool, int16_t> p;
      p.first = true;
      p.second = this->acc;
      return p;
    } else if (src ==  "NIL") {
      pair<bool, int16_t> p;
      p.first = true;
      p.second = 0;
      return p;
    } else if (src ==  "LEFT") {
      arrowType *a = arrows[3];
      pair<bool, int16_t> p;

      if (a->getRequest(nodeId)) {
        p.first = true;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = false;
      }
      a->updateValues();
      return p;
    } else if (src ==  "RIGHT") {
    	arrowType *a = arrows[1];
      pair<bool, int16_t> p;

      if (a->getRequest(nodeId)) {
        p.first = true;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = false;
      }
      a->updateValues();
      return p;
    } else if (src ==  "UP") {
    	arrowType *a = arrows[0];
	pair<bool, int16_t> p;

      if (a->getRequest(nodeId)){
        p.first = true;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = false;
      }
      a->updateValues();
      return p;
    } else if (src ==  "DOWN") {
    	arrowType *a = arrows[2];
      pair<bool, int16_t> p;

      if (a->getRequest(nodeId)) {
        p.first = true;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = false;
      }
      a->updateValues();
      return p;
    }
  }
  // ANY and LAST are for the future (hopefully not)
}

void node::first_instruction(){
  no_code=false;
  for(int i=0;i<inputCode.size();i++){
    if(!is_whitespace(inputCode[i])){
      pc=i;
      return;
    }
  }
  pc=0;
  no_code=true;
  return;
}

void node::reset(){
  acc=0;
  bak=0;
  first_instruction();
}

int node::getLine(std::string label){
  for(list<pair<string, uint8_t> >::iterator i=labels.begin(); i!=labels.end();i++){
    if(i->first==label)
      return i->second;
  }
  return -1;
}
