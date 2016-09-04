#include "spis.h"

node::node(uint8_t nId){
  nodeId = nId;
  acc = 0;
  bak = 0;
  pc=0;
  no_code=FALSE;
  for(int i = 0; i < 4; i++){
    arrows[i]=NULL;
  }
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
    else if(p.first!=READY)
      return true;
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
	arrowUpdate(a_num);
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
    if (p.first!=READY) {
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
 
    if(p.first==INVALID){
      //error
      return false;
    }

    else if (p.first!=READY) {
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
    int p;
    if(!strncmp("JRO", line.c_str(), 3)){
      string operands = line.substr(4);
      string src = operands.substr(0);
      pair<int8_t, int16_t> p = getFromSrc(src);
      if(p.first!=READY )
	 return true;
      else if(p.first==INVALID)
	return false;
      else
	pc=pc+p.second;
      return true;
    }
    else{
      p=getLine(line.substr(4));
      if(p<0)
	return false;
      if(strcmp("JGZ", line.c_str()) && strcmp("JLZ", line.c_str())
	 && strcmp("JEZ", line.c_str()) && strcmp("JNZ", line.c_str()))
	return false;
      
      else if(!strcmp("JGZ", line.c_str())&&this->acc>0)
	pc=p;
      else if(!strcmp("JLZ", line.c_str()) &&this->acc<0)
	pc=p;
      else if(!strcmp("JEZ", line.c_str())&&this->acc==0)
	pc=p;
      else if(!strcmp("JNZ", line.c_str())&&this->acc!=0)
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
  // TODO – Collect labels
  reset();
  labels.clear();
  code.clear();
  code.resize(inputCode.size());
  std::vector<int> labelLines;
  for (int8_t i = 0; i < code.size(); i++){
    bool space=false;
    string line=code[i];
    string tmp;
    int last_space=-1;
    int n_operands=0;
    bool label=false;
    for(unsigned int j=0;j<code[i].size();j++){
      char ch=line[j];
      if(ch==':'){
	if(last_space)
	  return false;
	else{
	  std::pair<std::string, uint8_t> p;
	  label=true;
	  p.first=line.substr(0,j-1);
	  p.second=i;
	  labels.push_back(p);
	  last_space=j;
	}
      }
      else if(ch==' '||j==code.size()-1){
	if(space==true)
	  continue;
	else{
	  bool symbol_found=false;
	  space=true;
	  tmp=line.substr(last_space+1, j-last_space-1);

	  if(label){
	    label=false;
	    code[i].append(tmp);
	    labelLines.push_back(i);
	  }
	  if(n_operands==0){
	    for (uint8_t x = 0; x < 4; x++) {
	      if (SIM_OPS[x].compare(tmp) == 0) {
		n_operands=0;
		symbol_found=true;
		goto operand_test;
		  }
	    }
	    for (uint8_t x = 0; x < 3; x++) {
	      if (SRC_OPS[x].compare(tmp) == 0) {
		symbol_found = true;
		n_operands=1;
		goto operand_test;
	      }
	    }
	    for (uint8_t x = 0; x < 4; x++) {
	      if (LAB_OPS[x].compare(tmp) == 0) {
		symbol_found = true;
	        label=true;
		//n_operands stays 0 - the test for labels happens later
		goto operand_test; 
	      }
	    }
	    if(tmp=="MOV"){
	      symbol_found==true;
	      n_operands=2;
	    }
	  }
	operand_test:
	  if(n_operands>0)
	    if(tmp=="UP"||tmp=="RIGHT"||tmp=="DOWN"||tmp=="LEFT"||tmp=="ACC"||tmp=="NIL"||(isNum(tmp)&&n_operands==2)){ //or number
	      n_operands--;
	       symbol_found=true;
	     }
	   if(!symbol_found)
	     return false;
	   else
	     code[i].append(tmp);
	}
	last_space=j;
      }
      else
	space=false;
    }
  }
  //Check no duplicate labels
  for(int i=0; i<labels.size();i++){
    for(int j=i+1;j<labels.size();j++){
      if(labels[i]==labels[j])
	return false;
    }
  }
  
  for(int i=0; i<labelLines.size();i++){
    string operand;
    bool label_exists=false;
    operand=code[labelLines[i]].substr(4);
    for(int j=0;j<labels.size();j++){
      if(labels[j].first==operand){
	label_exists=true;
	break;
      }
      if(label_exists==false)
	return false;
    }
  }
}/*
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
}
 */
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
      arrowUpdate(1);
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
  for(int i=0;i<code.size();i++){
    if(!is_whitespace(code[i])){
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

void node::arrowUpdate(unsigned int arrowID){
	string vals[2];
	arrow *tmp_arrow = arrows[arrowID];
	if(!arrows[arrowID]){
	   endwin();
	   printf("ERROR: arrow was NULL!\n");
	  return;
	}
	for(int i=0;i<2;i++){
	  if(arrows[arrowID]->status[i]==SET)
	    vals[i] = makeThreeDigit(tmp_arrow->value[0]);
	  else
	    vals[i] = " ?  ";
	}
	
	if (arrowID % 2==0) {
	  mvwprintw(tmp_arrow->win, 0, 0, vals[0].c_str());
	  mvwprintw(tmp_arrow->win, 0, 7, vals[1].c_str());
	} else {
	  mvwprintw(tmp_arrow->win, 0, 0, vals[0].c_str());
	  mvwprintw(tmp_arrow->win, 3, 0, vals[1].c_str());
	}
	wrefresh(tmp_arrow->win);
	return;  
}

int node::getLine(std::string label){
  //TODO
  return 0;
}
