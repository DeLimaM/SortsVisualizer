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

void selectionSortStep(sortParams *params) {
  if (params->state == SORT_STATE_FINISHED)
    return;

  int *array = params->array;
  int size = params->size;

  // initialize the sort
  if (params->state == SORT_STATE_IDLE) {
    params->i = 0;
    params->j = params->i + 1;
    params->index_min = params->i;
    params->state = SORT_STATE_RUNNING;
  }

  // find the minimum element
  if (params->j < size) {
    if (array[params->j] < array[params->index_min]) {
      params->index_min = params->j;
    }
    params->comparisons++;
    params->j++;
  }
  // swap the minimum element with the first element
  else {
    if (params->index_min != params->i) {
      swap(array[params->i], array[params->index_min]);
      setSwapIndex1(&params->swap_params, params->i);
      setSwapIndex2(&params->swap_params, params->index_min);
      params->swaps++;
    }
    params->i++;

    if (params->i >= size - 1) {
      params->state = SORT_STATE_FINISHED;
    } else {
      params->j = params->i + 1;
      params->index_min = params->i;
    }
  }
}

void insertionSortStep(sortParams *params) {
  if (params->state == SORT_STATE_FINISHED)
    return;

  int *array = params->array;
  int size = params->size;

  // initialize the sort
  if (params->state == SORT_STATE_IDLE) {
    params->i = 1;
    params->j = params->i;
    params->key = array[params->i];
    params->state = SORT_STATE_RUNNING;
  }

  // move the elements greater than key to position+1
  if (params->j > 0 && array[params->j - 1] > params->key) {
    array[params->j] = array[params->j - 1];
    setInsertIndex(&params->insert_params, params->j);
    params->comparisons++;
    params->inserts++;
    params->j--;
  }
  // place the key at the right position
  else {
    array[params->j] = params->key;
    setInsertIndex(&params->insert_params, params->j);
    params->i++;

    if (params->i >= size) {
      params->state = SORT_STATE_FINISHED;
    } else {
      params->j = params->i;
      params->key = array[params->i];
    }
  }
}