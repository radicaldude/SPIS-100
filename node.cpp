#include "spis.h"

node::node(uint8_t nId){
  this->nodeId = nId;
  this->acc = 0;
  this->bak = 0;
  this->pc=0;
  this->no_code=FALSE;
}

bool node::runline(){
  //string line = sanitize(code[this.pc]);
  if(inputCode.size()==0)
    return true;
  if(is_whitespace(inputCode[pc]) || inputCode[pc].length() == 0){
  	pc++;
  	if(pc>=inputCode.size()-1)
  	    pc=0;
    return true;
  }
  string line = inputCode[pc];
  // TODO – Remove labels and variations of commas, in sanitize function
  if(!strncmp("NOP", line.c_str(), 3)){
    pc++;
    return true;
  } else if(!strncmp("MOV", line.c_str(), 3)){
    int c;
    std::string src, dst;
    pair<int8_t, uint16_t> p;
    int16_t input;
    arrow *a;

    string operands = line.substr(4);
    c = operands.find_first_of(' ');
    src = operands.substr(0,c);
    dst = operands.substr(c + 1);
    p = getFromSrc(src);

    if(p.first==INVALID)
      return false;
    else if(p.first==WAIT)
      return true;
    input = p.second;
    if(!strncmp(dst.c_str(), "ACC", 3)) {
      acc = input;
    } else if(!strncmp(dst.c_str(), "NIL",3)) {
	
    }else{
      if(!strncmp("LEFT", dst.c_str(), 3)){
	a = arrows[3];
      } else if(!strncmp(dst.c_str(), "RIG", 3)) {
	a = arrows[1];
      } else if(strncmp(dst.c_str(), "UP",2)) {
	a = arrows[0];
      } else if(strncmp(dst.c_str(),"DOW", 3)) {
    	a = arrows[2];
      }
      else
	return false;
      if (!a->setRequest(nodeId)){
	a->nodeSet(nodeId, input);
	updateArrow((a-arrows)/sizeof(arrow));
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
    int16_t input;
    if (p.first==WAIT) {
      return true;
    }
    
    else if(p.first==INVALID){
      //error
      return false;
    }
    else{
      input = p.second;
      acc += input;
      pc++;
      return true;
    }
  } else if(!strncmp("SUB", line.c_str(), 3)){
    string src = line.substr(4);
    pair<int8_t, int16_t> p = getFromSrc(src);
    
    int16_t input;

        
    if (p.first==WAIT) {
      return true;
    }
    
    else if(p.first==INVALID){
      //error
      return false;
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
    pair<int8_t, int16_t> p;
    p=getFromSrc(line.substr(4));
    if(!strncmp("JRO", line.c_str(), 3)){
      if(p.first==WAIT)
	 return true;
       else if(p.first==SET)
	 pc=pc+p.second;
       return true;
    pc=pc+p.second;
    } 
    if(p.second==this->labels.end()->second)
      return false;
    if(strncmp("JGZ", line.c_str(), 3) && strncmp("JLZ", line.c_str(),3)
       && strncmp("JEZ", line.c_str(),3) && strncmp("JNZ", line.c_str(),3))
      return false;
      
    else if(!strncmp("JGZ", line.c_str(), 3)&&this->acc>0)
      pc=p.second;
    else if(!strncmp("JLZ", line.c_str(),3) &&this->acc<0)
      pc=p.second;
    else if(!strncmp("JEZ", line.c_str(),3)&&this->acc==0)
      pc=p.second;
    else if(!strncmp("JNZ", line.c_str(),3)&&this->acc!=0)
      pc=p.second;
    else
      pc++;
    return true;
  }
  else
    return false;
}
              
bool node::runPrepare(){
/*    // TODO – Sanitize
  
  // TODO – Collect labels
  labels.clear();
  
  for (int8_t i = 0; i < code.size(); i++) {
    uint8_t colonIndex = code[i].find(':');
    if(colonIndex != string::npos) {
      string labelName = code[i].substr(0, colonIndex);
      if(input.find_first_not_of(" ,") != std::string::npos) {
        // TODO – Handle error
      }
      labels.insert(pair<string, int8_t>(labelName, i));
    }
  }
  // TODO – Remove labels from code
  
  // TODO – Debug
  codeLoop: for (uint8_t i = 0; i < code.size(); i++) {
    string op = code[i].substr(0, 3);
    bool opFound = false;
    
    for (uint8_t x = 0; x < 4; x++) {
      if (SIM_OPS[x].compare(op) == 0) {
        opFound = true;
      }
    }
    if (opFound == true) {
      if (code[i].length()) {
        // TODO – Handle error
      }
      continue codeLoop;
    }
    
    
    for (uint8_t x = 0; x < 3; x++) {
      if (SRC_OPS[x].compare(op) == 0) {
        opFound = true;
      }
    }
    if (opFound == true) {
      string param = code[i].substr(4);
      if (param.find(' ') != string::npos) {
        // TODO – Handle error
      } else if (!testSrc(param)) {
        // TODO – Handle error
      }
      continue codeLoop;
    }
    
    if (SD_OPS[0].compare(op) == 0) {
      string param = code[i].substr(4);
      string paramSrc = param.substr(0, param.find(' '));
      string paramDst = param.substr(paramSrc.length() + 1)
      if(paramDst.find(' ') != string::npos) {
        // TODO – Handle error
      } else if (!testSrc(paramSrc)) {
        // TODO – Handle error
      } else if (!testDst(paramDst)) {
        // TODO – Handle error
      }
      continue codeLoop;
    }
    
    for (uint8_t x = 0; x < 4; x++) {
      if (LAB_OPS[x].compare(op) == 0) {
        opFound = true;
      }
    }
    if (opFound == true) {
      string param = code[i].substr(4);
      if (labels.find(param) == map::end) {
        // TODO - Handle error
      }
      continue codeLoop;
    }
    }
  
  // TODO – Set properties for run line*/
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
    p.first = SET;
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
      p.first = SET;
      p.second = this->acc;
      return p;
    } else if (src ==  "NIL") {
      pair<bool, int16_t> p;
      p.first = SET;
      p.second = 0;
      return p;
    } else if (src ==  "LEFT") {
      arrow *a = arrows[3];
      pair<bool, int16_t> p;

      if (a->getRequest(nodeId)) {
        p.first = SET;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = WAIT;
      }
      arrowUpdate(3);
      return p;
    } else if (src ==  "RIGHT") {
      arrow *a = arrows[1];
      pair<bool, int16_t> p;

      if (a->getRequest(nodeId)) {
        p.first = SET;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = WAIT;
      }
      arrowUpdate[1];
      return p;
    } else if (src ==  "UP") {
      arrow *a = arrows[0];
      pair<bool, int16_t> p;

      if (a->getRequest(nodeId)){
        p.first = SET;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = WAIT;
      }
      arrowUpdate(0);
      return p;
    } else if (src ==  "DOWN") {
      arrow *a = arrows[2];
      pair<bool, int16_t> p;

      if (a->getRequest(nodeId)) {
        p.first = SET;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = WAIT;
      }
      arrowUpdate(2);
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

void node::updateArrow(int arrowID){
	string vals[2];
	if(arrows[arrowID]){
	  endwin();
	  printf("ERROR: arrow was NULL!\n");
	  return;
	}
	for(int i=0;i<2;i++){
	  if(arrows[arrowID].status[i]=SET)
	    vals[i] = tmp_arrow->value[0].c_str();
	  else
	    vals[i] = " ? ";
	}
	
	if (arrowIndex & 2) {
	  mvwprintw(tmp_arrow->win, 0, 0, makeThreeDigit(vals[0]).c_str());
	  mvwprintw(tmp_arrow->win, 0, 7, makeThreeDigit(vals[1]).c_str());
	} else {
	  mvwprintw(tmp_arrow->win, 0, 0, makeThreeDigit(vals[0]).c_str());
	  mvwprintw(tmp_arrow->win, 3, 0, makeThreeDigit(vals[1]).c_str());
	}
	wrefresh(tmp_arrow->win);
	return;  
}
