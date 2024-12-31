#include "sortviz.h"
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

  if (params->base.state == SORT_STATE_IDLE) {
    params->stackSize = 1;
    params->stack[0] = (MergeState){0, 0, params->base.size - 1, 0};
    params->tempArray = malloc(params->base.size * sizeof(int));
    memcpy(params->tempArray, params->base.array,
           params->base.size * sizeof(int));
    params->base.state = SORT_STATE_RUNNING;
  }

  while (params->stackSize > 0) {
    MergeState *current = &params->stack[params->stackSize - 1];

    if (current->left >= current->right) {
      params->stackSize--;
      continue;
    }

    switch (current->step) {
    case INITIAL_SPLIT:
      current->mid = (current->left + current->right) / 2;
      current->step = LEFT_HALF;
      params->stack[params->stackSize++] =
          (MergeState){current->left, 0, current->mid, 0};
      return;

    case LEFT_HALF:
      current->step = RIGHT_HALF;
      params->stack[params->stackSize++] =
          (MergeState){current->mid + 1, 0, current->right, 0};
      return;

    case RIGHT_HALF:
      current->step = MERGE_STEP;
      params->mergeIndex = current->left;
      params->leftIndex = current->left;
      params->rightIndex = current->mid + 1;
      memcpy(params->tempArray + current->left,
             params->base.array + current->left,
             (current->right - current->left + 1) * sizeof(int));
      return;

    case MERGE_STEP:
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

void quickSortStep(QuickSortParams *params) {
  if (params->base.state == SORT_STATE_FINISHED)
    return;

  if (params->base.state == SORT_STATE_IDLE) {
    params->stackSize = 1;
    params->stack[0] = (QuickState){
        0, params->base.size - 1, params->base.size - 1, 0, 0, PARTITION_START};
    params->base.state = SORT_STATE_RUNNING;
  }

  while (params->stackSize > 0) {
    QuickState *current = &params->stack[params->stackSize - 1];

    if (current->left >= current->right) {
      params->stackSize--;
      continue;
    }

    switch (current->step) {
    case PARTITION_START:
      current->pivotIndex = current->right;
      current->i = current->left - 1;
      current->j = current->left;
      current->step = PARTITION_PROCESS;
      return;

    case PARTITION_PROCESS:
      if (current->j < current->right) {
        params->base.comparisons++;
        if (params->base.array[current->j] <=
            params->base.array[current->pivotIndex]) {
          current->i++;
          if (current->i != current->j) {
            int temp = params->base.array[current->i];
            params->base.array[current->i] = params->base.array[current->j];
            params->base.array[current->j] = temp;
            params->base.swaps++;
            setSwapIndex(&params->base.swap_params, current->i, current->j);
          }
        }
        current->j++;
        return;
      }

      current->i++;
      if (current->i != current->pivotIndex) {
        int temp = params->base.array[current->i];
        params->base.array[current->i] =
            params->base.array[current->pivotIndex];
        params->base.array[current->pivotIndex] = temp;
        params->base.swaps++;
        setSwapIndex(&params->base.swap_params, current->i,
                     current->pivotIndex);
      }
      current->step = HANDLE_LEFT;
      return;

    case HANDLE_LEFT:
      current->step = HANDLE_RIGHT;
      if (current->left < current->i - 1) {
        params->stack[params->stackSize++] = (QuickState){
            current->left, current->i - 1, 0, 0, 0, PARTITION_START};
      }
      return;

    case HANDLE_RIGHT:
      params->stackSize--;
      if (current->i + 1 < current->right) {
        params->stack[params->stackSize++] = (QuickState){
            current->i + 1, current->right, 0, 0, 0, PARTITION_START};
      }
      if (params->stackSize == 0) {
        params->base.state = SORT_STATE_FINISHED;
      }
      return;
    }
  }
}