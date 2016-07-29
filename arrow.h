#include "spis.h"

using namespace std

class arrow{
  uint8_t getNodeIndex(int8_t nodeId);

public:
  int8_t nodeId[2];
  int8_t status[2];
  int16_t value[2];

  WINDOW *win;
  arrow(int8_t nodeId1, int8_t nodeId2);
  bool nodeRequest(int8_t nodeId);
  int16_t nodeGet(int8_t nodeId);
  void nodeSet(int8_t nodeId, int16_t number);
};

arrow::arrow(int8_t nodeId1, int8_t nodeId2) {
  nodeId[0] = nodeId1;
  nodeId[1] = nodeId2;

  status[0] = 0;
  status[1] = 0;

  value[0] = 0;
  value[1] = 0;
}

uint8_t arrow::getNodeIndex(int8_t nId) {
  uint8_t nodeIndex = 0;
  if (nId == nodeId[1]) {
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
  return false;
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

  return;
}
