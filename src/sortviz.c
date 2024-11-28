#include "sortviz.h"
#include "gui.h"
#include "sorts.h"
#include "terminal.h"

interfaceType interface = TERMINAL;

void setInterface(interfaceType type) { interface = type; }

void setInsertIndex(insertParams *params, int new_index) {
  params->prev_index = params->index;
  params->index = new_index;
}

void setSwapIndex1(swapParams *params, int new_index1) {
  params->prev_index1 = params->index1;
  params->index1 = new_index1;
}

void setSwapIndex2(swapParams *params, int new_index2) {
  params->prev_index2 = params->index2;
  params->index2 = new_index2;
}

void doSort(sortType type, int sleep_time) {
  switch (interface) {
  case TERMINAL:
    doSortInTerminal(type, sleep_time);
    break;
  case GUI:
    doSortInGUI(type, sleep_time);
    break;
  }
}