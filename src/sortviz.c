#include "sortviz.h"
#include "gui.h"
#include "sorts.h"
#include "terminal.h"

void initBaseParams(BaseSortParams *params, int size, SortType type) {
  params->array = createShuffledArray(size);
  params->size = size;
  params->type = type;
  params->state = SORT_STATE_IDLE;
  params->swaps = 0;
  params->inserts = 0;
  params->comparisons = 0;

  params->swap_params.index1 = -1;
  params->swap_params.index2 = -1;
  params->swap_params.prev_index1 = -1;
  params->swap_params.prev_index2 = -1;

  params->insert_params.index = -1;
  params->insert_params.prev_index = -1;
}

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

void doSort(SortType type, InterfaceType interface, int size) {
  switch (interface) {
  case TERMINAL:
    doSortInTerminal(type);
    break;
  case GUI:
    doSortInGUI(type, size);
    break;
  }
}