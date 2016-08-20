#include "arrow.h"
#include <map>
#include <cstdlib>

using namespace std;

const string SIM_OPS[] = { "NOP", "SWP", "SAV", "NEG" };  // Operations with no parameters
const string SRC_OPS[] = { "ADD", "SUB", "JRO" };  // Operations with only SRC as a parameter
const string SD_OPS[] = { "MOV" };  // Operations with both SRC and DST as parameters
const string LAB_OPS[] = { "JMP", "JEZ", "JNZ", "JGZ", "JLZ"}; // Operations with only SRC as a parameter

class io{
 public:
  arrow Arrow; 
  std::vector<string> io;
};

class node{
  uint8_t nodeId;
  pair<int8_t, int16_t> getFromSrc(string src);
 public:
  WINDOW *w_main;
  WINDOW *w_code;
  WINDOW *w_reg;
  node(uint8_t nId);
  int16_t acc;
  int16_t bak;
  int8_t  pc;
  std::vector<string> code;
  bool runline();
  bool runPrepare();
  std::map<uint8_t, uint16_t> labels;
  arrow *arrows[4];
};

node::node(uint8_t nId){
  this->nodeId = nId;
  this->acc = 0;
  this->bak = 0;
}

bool node::runline(){
  //string line = sanitize(code[this.pc]);
  string line = code[pc];
  // TODO – Remove labels and variations of commas, in sanitize function
  
  if(strncmp("NOP", line.c_str(), 3)){
    pc++;
    return true;
  } else if(strncmp("MOV", line.c_str(), 3)){
    int c;
    std::string src, dst;
    pair<int8_t, uint16_t> p;
    int16_t input;

    string operands = line.substr(4);
    c = operands.find_first_of(' ');
    src = operands.substr(0,c);
    dst = operands.substr(c);
    p = getFromSrc(src);
    if(p.first==INVALID)
      return false;
    input = p.second;
    if(dst == "ACC") {
      acc = input;
    } else if(dst== "NIL") {
      pc++;
      return true;
      // T:I guess do nothing??? J: nope
    } else if(dst== "LEFT"){
      arrow *a = arrows[3];
      if (!a->nodeRequest(nodeId)) {
	a->nodeSet(nodeId, input);
      } else {
        pc--;
      }
    } else if(dst== "RIGHT") {
      arrow *a = arrows[1];

      if (!a->nodeRequest(nodeId)) {
        a->nodeSet(nodeId, input);
      } else {
        pc--;
      }
    } else if(dst== "UP") {
      arrow *a = arrows[0];

      if (!a->nodeRequest(nodeId)) {
        a->nodeSet(nodeId, input);
      } else {
        pc--;
      }
    } else if(dst== "DOWN") {
      arrow *a = arrows[2];

      if (!a->nodeRequest(nodeId)) {
        a->nodeSet(nodeId, input);
      } else {
        pc--;
      }
      // TODO – Handle error
      return false;
    }
    pc++;
    return true;

  } else if(strncmp("SWP", line.c_str(), 3)){
    int16_t tmpAcc = acc;
    acc = bak;
    bak = tmpAcc;
    pc++;
    return true;

  } else if(strncmp("SAV", line.c_str(), 3)){
    bak = acc;
    pc++;
    return true;

  } else if(strncmp("ADD", line.c_str(), 3)){
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
  } else if(strncmp("SUB", line.c_str(), 3)){
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
    }
    
  } else if(strncmp("NEG", line.c_str(), 3)){
    acc = -acc;
    pc++;
    return true;   
  }
  else if(strncmp("J",line.c_str(),1)){
    pair<int8_t, int16_t> p;
    p=getFromSrc(line.substr(4));
    if(strncmp("JRO", line.c_str(), 3)){
      if(p.first==WAIT)
	 return true;
       else if(p.first==SET)
	 pc=pc+p.second;
       return true;
    pc=pc+p.second;
    } 
    if(p.second==this->labels.end()->second)
      return false;
    if(strncmp("JGZ", line.c_str(), 3)&&this->acc>0)
      pc=p.second;
    else if(strncmp("JLZ", line.c_str(),3) &&this->acc<0)
      pc=p.second;
    else if(strncmp("JEZ", line.c_str(),3)&&this->acc==0)
      pc=p.second;
    else if(strncmp("JNZ", line.c_str(),3)&&this->acc!=0)
      pc=p.second;
    else
      pc++;
    return true;
  }
  else
    // TODO – Handle error
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

      if (a->nodeRequest(nodeId)) {
        p.first = SET;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = WAIT;
      }
      return p;
    } else if (src ==  "RIGHT") {
      arrow *a = arrows[1];
      pair<bool, int16_t> p;

      if (a->nodeRequest(nodeId)) {
        p.first = SET;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = WAIT;
      }
      return p;
    } else if (src ==  "UP") {
      arrow *a = arrows[0];
      pair<bool, int16_t> p;

      if (a->nodeRequest(nodeId)){
        p.first = SET;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = WAIT;
      }
      return p;
    } else if (src ==  "DOWN") {
      arrow *a = arrows[2];
      pair<bool, int16_t> p;

      if (a->nodeRequest(nodeId)) {
        p.first = SET;
        p.second = a->nodeGet(nodeId);
      } else {
        p.first = WAIT;
      }
      return p;
    }
  }
  // ANY and LAST are for the future (hopefully not)
}
