#include "spis.h"

class arrow{
  uint8_t getNodeIndex(int8_t nodeId);
  
public:
  int8_t nodeId[2];
  int8_t status[2];
  int16_t value[2];
  
  arrow(int8_t nodeId1, int8_t nodeId2);
  bool nodeRequest(int8_t nodeId);
  int16_t nodeGet(int8_t nodeId);
  void nodeSet(int8_t nodeId, int16_t number);
};

void arrow::arrow(int8_t nodeId1, int8_t nodeId2) {
  nodeId[0] = nodeId1;
  nodeId[1] = nodeId2;
  
  status[0] = 0;
  status[1] = 0;
  
  value[0] = 0;
  value[1] = 0;
}

uint8_t arrow::getNodeIndex(int8_t nodeId) {
  uint8_t nodeIndex = 0;
  if (nodeId == nodeId[1]) {
    nodeIndex = 1;
  }
  
  return nodeIndex;
}

bool arrow::nodeRequest(int8_t nodeId) {
  uint8_t nodeIndex = getNodeIndex(nodeId);
  // TODO – Define status values
  if (status[nodeIndex] == 1) {
    return true;
  }
  return false
}

int16_t arrow::nodeGet(int8_t nodeId) {
  uint8_t nodeIndex = getNodeIndex(nodeId);
  int16_t tmpValue = value[nodeIndex];
  
  // TODO – Define status values
  status[nodeIndex] = 0;
  
  return tmpValue;
}

void arrow::nodeSet(int8_t nodeId, int16_t number) {
  uint8_t nodeIndex = getNodeIndex(nodeId);
  
  value[nodeIndex] = number;
  // TODO – Define status values
  status[nodeIndex] = 1;
  
  return tmpValue
}



class node{
  uint8_t nodeId;
  arrow arrows[];
  
  pair<bool, int16_t> getFromSrc(string src);
  
public:
  node(uint8_t nodeId, arrow arrowArray[]);
  int16_t acc;
  int16_t bak;
  int8_t  pc;
  std::vector<string> code;

  bool runline();  
};

void node::node(uint8_t nId, arrow arrowArray[]) {
  nodeId = nId;
  arrows = arrowArray[]
  // TODO – Ask Joe how to refer to local variables
  acc = 0;
  bak = 0;
  // TODO – Not sure how a vector really works :\
}

bool node::runline(){
  string line = sanitize(code[this.pc]);
  // TODO – Remove labels and variations of commas, in sanitize function
  
  if(strncmp("NOP", line.c_str(), 3)){
    pc++;
    return true;
  } else if(strncmp("MOV", line.c_str(), 3)){
    int c;
    std::string src, dst;
    pair<bool, uint16_t> p;
    int16_t input;
    
    operands = line.sub_str(4);
    c=operands.find_first_of(' ');
    src = operands.substr(0,c);
    dst = operands.substr(c);
    p = getFromSrc(src);
    if (!p.first) 
      return true;
    
    input = p.second;
    
    if(dst == "ACC") {
      acc = input;
    } else if(dst== "NIL") {
      // I guess do nothing???
    } else if(dst== "LEFT":
      arrow a = arrows[3];
      
      if (!a.nodeRequest(nodeId)) {
        a.nodeSet(nodeId, input);
      } else {
        pc--;
      }
    } else if(dst== "RIGHT") {
      arrow a = arrows[1];
      
      if (!a.nodeRequest(nodeId)) {
        a.nodeSet(nodeId, input);
      } else {
        pc--;
      }
    } else if(dst== "UP") {
      arrow a = arrows[0];
      
      if (!a.nodeRequest(nodeId)) {
        a.nodeSet(nodeId, input);
      } else {
        pc--;
      }
    } else if(dst== "DOWN") {
      arrow a = arrows[2];
      
      if (!a.nodeRequest(nodeId)) {
        a.nodeSet(nodeId, input);
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
    return true;
    
  } else if(strncmp("SAV", line.c_str(), 3)){
    bak = acc;
    return true;
    
  } else if(strncmp("ADD", line.c_str(), 3)){
    string src = line.sub_str(4);
    pair<bool, int16_t> p = getFromSrc(src);
    int16_t input;
    
    if (!p.first) {
      return false;
    }
    
    input = p.second;
    acc += input;
    return true;
    
  } else if(strncmp("SUB", line.c_str(), 3)){
    string src = line.sub_str(4);
    pair<bool, int16_t> p = getFromSrc(src);
    int16_t input;
    
    if (!p.first) {
      return false;
    }
    
    input = p.second;
    
    acc -= input;
    return true;
    
  } else if(strncmp("NEG", line.c_str(), 3)){
    acc = -acc;
    return true;
    
  } else if(strncmp("JRO", line.c_str(), 3)){
    string src = line.sub_str(4);
    pair<bool, int16_t> p = getFromSrc(src);
    int16_t input;
    
    if (!p.first) {
      return false;
    }
    
    input = p.second;
    
    pc += input;
    return true;
    
  } else if(strncmp("J", line.c_str(), 1)){
    // JMP, JEZ, JNZ, JGZ, JLZ
  } else {
    // TODO – Handle error
  }
}

pair<bool, int16_t> node::getFromSrc(string src) {
  if(src.find_first_not_of("0123456789") == std::string::npos) {
    // Then src is just a number
    pair<bool, int16_t> p;
    p.first = true;
    p.second = stoi(src);
    
    return p;
  } else {
    if (src == "ACC") {
      pair<bool, int16_t> p;
      p.first = true;
      p.second = acc;
      return p;
    } else if (src ==  "NIL") {
      pair<bool, int16_t> p;
      p.first = true;
      p.second = 0;
      return p;
    } else if (src ==  "LEFT") {
      arrow = arrows[3];
      pair<bool, int16_t> p;
      
      if (arrow.nodeRequest(nodeId)) {
        p.first = true;
        p.second = arrow.nodeGet(nodeId);
      } else {
        p.first = false;
      }
      return p;
    } else if (src ==  "RIGHT") {
      arrow a = arrows[1];
      pair<bool, int16_t> p;
      
      if (a.nodeRequest(nodeId)) {
        p.first = true;
        p.second = a.nodeGet(nodeId);
      } else {
        p.first = false;
      }
      return p;
    } else if (src ==  "UP") {
      arrow a = arrows[0];
      pair<bool, int16_t> p;
      
      if (a.nodeRequest(nodeId)) {
        p.first = true;
        p.second = a.nodeGet(nodeId);
      } else {
        p.first = false;
      }
      return p;
    } else if (src ==  "DOWN") {
      arrow a = arrows[2];
      pair<bool, int16_t> p;
      
      if (a.nodeRequest(nodeId)) {
        p.first = true;
        p.second = a.nodeGet(nodeId);
      } else {
        p.first = false;
      }
      return p;
    } else {
      // TODO – Handle error
    }
  }
  // ANY and LAST are for the future (hopefully not)
}
