#include "spis.h"

arrow::arrow(int8_t nodeId1, int8_t nodeId2) {
  nodeId[0] = nodeId1;
  nodeId[1] = nodeId2;

  status[0] = WAIT;
  status[1] = WAIT;

  value[0] = 0;
  value[1] = 0;
}

uint8_t arrowType::getNodeIndex(int8_t nId) {
  uint8_t nodeIndex = 0;
  if (nId == nodeId[1])
    nodeIndex = 1;
  return nodeIndex;
}

bool arrowType::setRequest(int8_t nodeId) {
  return false;
}

bool arrowType::getRequest(int8_t nodeId) {
  return false;
}

int16_t arrowType::nodeGet(int8_t nodeId) {
  return 0;
}

void arrowType::nodeSet(int8_t nodeId, int16_t number) {
  return;
}

bool arrow::setRequest(int8_t nodeId) {
  uint8_t nodeIndex = getNodeIndex(nodeId);
  // TODO – Define status values
  if (status[nodeIndex] == WAIT) {
    return true;
  }
  return false;
}

bool arrow::getRequest(int8_t nodeId) {
  uint8_t nodeIndex = getNodeIndex(nodeId);
  if (status[(nodeIndex + 1) % 2] == SET) {
    return true;
  }
  return false;
}

int16_t arrow::nodeGet(int8_t nodeId) {
  uint8_t nodeIndex = (getNodeIndex(nodeId) + 1) % 2;
  int16_t tmpValue = value[nodeIndex];

  status[nodeIndex] = WAIT;
  return tmpValue;
}

void arrow::nodeSet(int8_t nodeId, int16_t number) {
  uint8_t nodeIndex = getNodeIndex(nodeId);

  value[nodeIndex] = number;
  status[nodeIndex] = SET;
  return;
}


