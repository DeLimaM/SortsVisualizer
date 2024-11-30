#include "sortviz.h"
#include "gui.h"
#include "sorts.h"
#include "terminal.h"

InterfaceType interface = TERMINAL;

void shuffle(int *array, int size) {
  srand(time(NULL));
  for (int i = size - 1; i > 0; i--) {
    int j = random(0, i);
    swap(array[i], array[j]);
  }
}

int *createShuffledArray(int size) {
  int *array = malloc(size * sizeof(int));
  for (int i = 0; i < size; i++) {
    array[i] = i + 1;
  }
  shuffle(array, size);
  return array;
}

void setInterface(InterfaceType type) { interface = type; }

void setInsertIndex(InsertParams *params, int new_index) {
  params->prev_index = params->index;
  params->index = new_index;
}

void setSwapIndex(SwapParams *params, int new_index1, int new_index2) {
  params->prev_index1 = params->index1;
  params->prev_index2 = params->index2;
  params->index1 = new_index1;
  params->index2 = new_index2;
}

void doSort(SortType type, int sleep_time) {
  switch (interface) {
  case TERMINAL:
    doSortInTerminal(type, sleep_time);
    break;
  case GUI:
    doSortInGUI(type, sleep_time);
    break;
  }
}