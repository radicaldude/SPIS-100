#include "spis.h"

arrow::arrow(int8_t nodeId1, int8_t nodeId2) {
  nodeId[0] = nodeId1;
  nodeId[1] = nodeId2;

  status[0] = WAIT;
  status[1] = WAIT;

  value[0] = 0;
  value[1] = 0;
}

uint8_t arrowType::getNodeIndex(int8_t id) {
  uint8_t nodeIndex = 0;
  if (id == nodeId[1])
    nodeIndex = 1;
  return nodeIndex;
}


// arrow

bool arrow::setRequest(int8_t id) {
  uint8_t nodeIndex = getNodeIndex(id);
  // TODO – Define status values
  if (status[nodeIndex] == WAIT) {
    return true;
  }
  return false;
}

bool arrow::getRequest(int8_t id) {
  uint8_t nodeIndex = getNodeIndex(id);
  if (status[(nodeIndex + 1) % 2] == SET) {
    return true;
  }
  return false;
}

int16_t arrow::nodeGet(int8_t id) {
  uint8_t nodeIndex = (getNodeIndex(id) + 1) % 2;
  int16_t tmpValue = value[nodeIndex];

  status[nodeIndex] = WAIT;
  return tmpValue;
}

void arrow::nodeSet(int8_t id, int16_t number) {
  uint8_t nodeIndex = getNodeIndex(id);
  
  value[nodeIndex] = number;
  status[nodeIndex] = SET;
  return;
}

void arrow::tickUpdate() {
	for(int k=0;k<2;k++)
		if(status[k]==SET)
			status[k]==READY;

	return;
}


// inputArrow

inputArrow::inputArrow(int8_t id) {
	nodeId[0] = id;

	status[0] = WAIT;

	value[0] = 0;

	return;
}

bool inputArrow::setRequest(int8_t id) {
	if (status[0] == WAIT && id < 0) {
		return true;
	}
	return false;
}

bool inputArrow::getRequest(int8_t id) {
	if (status[0] == READY) {
		return true;
	}
	return false;
}

int16_t inputArrow::nodeGet(int8_t id) {
  status[0] = WAIT;
  return value[0];
}

void inputArrow::nodeSet(int8_t id, int16_t number) {
  value[0] = number;
  status[0] = SET;
  return;
}


// outputArrow

outputArrow::outputArrow(int8_t id) {
	nodeId[0] = id;

	status[0] = WAIT;

	value[0] = 0;

	return;
}

bool outputArrow::setRequest(int8_t id) {
	if (status[0] == WAIT) {
		return true;
	}
	return false;
}

bool outputArrow::getRequest(int8_t id) {
	if (status[0] == SET && id < 0) {
		return true;
	}
	return false;
}

int16_t outputArrow::nodeGet(int8_t id) {
  status[0] = WAIT;
  return value[0];
}

void outputArrow::nodeSet(int8_t id, int16_t number) {
  value[0] = number;
  status[0] = SET;
  return;
}
