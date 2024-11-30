#include "sortviz.h"
#include "utils.h"
#include <string.h>

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

void mergeSortStep(MergeSortParams *params) {
  if (params->base.state == SORT_STATE_FINISHED)
    return;

  // Initialize if starting
  if (params->base.state == SORT_STATE_IDLE) {
    params->stackSize = 1;
    params->stack[0] = (MergeState){0, 0, params->base.size - 1, 0};
    params->tempArray = malloc(params->base.size * sizeof(int));
    params->base.state = SORT_STATE_RUNNING;
  }

  // Process current state
  while (params->stackSize > 0) {
    MergeState *current = &params->stack[params->stackSize - 1];

    if (current->left >= current->right) {
      params->stackSize--;
      continue;
    }

    switch (current->step) {
    case 0: // Initial split
      current->mid = (current->left + current->right) / 2;
      current->step = 1;
      params->stack[params->stackSize++] =
          (MergeState){current->left, 0, current->mid, 0};
      return;

    case 1: // Process right half
      current->step = 2;
      params->stack[params->stackSize++] =
          (MergeState){current->mid + 1, 0, current->right, 0};
      return;

    case 2: // Start merging
      current->step = 3;
      params->mergeIndex = current->left;
      params->leftIndex = current->left;
      params->rightIndex = current->mid + 1;
      memcpy(params->tempArray + current->left,
             params->base.array + current->left,
             (current->right - current->left + 1) * sizeof(int));
      return;

    case 3: // Merge step by step
      if (params->mergeIndex <= current->right) {
        bool takeLeft = params->rightIndex > current->right ||
                        (params->leftIndex <= current->mid &&
                         params->tempArray[params->leftIndex] <=
                             params->tempArray[params->rightIndex]);

        if (takeLeft) {
          params->base.array[params->mergeIndex] =
              params->tempArray[params->leftIndex++];
        } else {
          params->base.array[params->mergeIndex] =
              params->tempArray[params->rightIndex++];
        }

        setInsertIndex(&params->base.insert_params, params->mergeIndex);
        params->base.comparisons++;
        params->base.inserts++;
        params->mergeIndex++;
        return;
      }
      params->stackSize--;
      if (params->stackSize == 0) {
        params->base.state = SORT_STATE_FINISHED;
        free(params->tempArray);
      }
      return;
    }
  }
}