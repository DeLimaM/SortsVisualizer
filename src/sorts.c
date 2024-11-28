#include "sortviz.h"
#include "utils.h"

void bubbleSortStep(sortParams *params) {
  if (params->state == SORT_STATE_FINISHED)
    return;

  int *array = params->array;
  int size = params->size;

  // initialize the sort
  if (params->state == SORT_STATE_IDLE) {
    params->i = 0;
    params->j = 0;
    params->state = SORT_STATE_RUNNING;
  }

  // do one step of the sort (compare and swap)
  if (params->j < size - params->i - 1) {
    if (array[params->j] > array[params->j + 1]) {
      swap(array[params->j], array[params->j + 1]);
      setSwapIndex1(&params->swap_params, params->j);
      setSwapIndex2(&params->swap_params, params->j + 1);
      params->swaps++;
    }
    params->comparisons++;
    params->j++;
  } else {
    params->j = 0;
    params->i++;
    if (params->i >= size - 1) {
      params->state = SORT_STATE_FINISHED;
    }
  }
}