#include "sortviz.h"
#include "utils.h"

void bubbleSortStep(BubbleSortParams *params) {
  if (params->base.state == SORT_STATE_FINISHED)
    return;

  int *array = params->base.array;
  int size = params->base.size;

  if (params->base.state == SORT_STATE_IDLE) {
    params->i = 0;
    params->j = 0;
    params->base.state = SORT_STATE_RUNNING;
  }

  if (params->j < size - params->i - 1) {
    if (array[params->j] > array[params->j + 1]) {
      swap(array[params->j], array[params->j + 1]);
      setSwapIndex(&params->base.swap_params, params->j, params->j + 1);
      params->base.swaps++;
    }
    params->base.comparisons++;
    params->j++;
  } else {
    params->j = 0;
    params->i++;
    if (params->i >= size - 1) {
      params->base.state = SORT_STATE_FINISHED;
    }
  }
}

void selectionSortStep(SelectionSortParams *params) {
  if (params->base.state == SORT_STATE_FINISHED)
    return;

  int *array = params->base.array;
  int size = params->base.size;

  if (params->base.state == SORT_STATE_IDLE) {
    params->i = 0;
    params->j = params->i + 1;
    params->index_min = params->i;
    params->base.state = SORT_STATE_RUNNING;
  }

  if (params->j < size) {
    if (array[params->j] < array[params->index_min]) {
      params->index_min = params->j;
    }
    params->base.comparisons++;
    params->j++;
  } else {
    if (params->index_min != params->i) {
      swap(array[params->i], array[params->index_min]);
      setSwapIndex(&params->base.swap_params, params->i, params->index_min);
      params->base.swaps++;
    }
    params->i++;
    if (params->i >= size - 1) {
      params->base.state = SORT_STATE_FINISHED;
    } else {
      params->j = params->i + 1;
      params->index_min = params->i;
    }
  }
}

void insertionSortStep(InsertionSortParams *params) {
  if (params->base.state == SORT_STATE_FINISHED)
    return;

  int *array = params->base.array;
  int size = params->base.size;

  if (params->base.state == SORT_STATE_IDLE) {
    params->i = 1;
    params->j = params->i;
    params->key = array[params->i];
    params->base.state = SORT_STATE_RUNNING;
  }

  if (params->j > 0 && array[params->j - 1] > params->key) {
    array[params->j] = array[params->j - 1];
    setInsertIndex(&params->base.insert_params, params->j);
    params->base.comparisons++;
    params->j--;
  } else {
    array[params->j] = params->key;
    setInsertIndex(&params->base.insert_params, params->j);
    params->base.inserts++;

    params->i++;
    if (params->i >= size) {
      params->base.state = SORT_STATE_FINISHED;
    } else {
      params->j = params->i;
      params->key = array[params->i];
    }
  }
}