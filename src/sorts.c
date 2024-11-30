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

void mergeSortStep(MergeSortParams *params) {
  if (params->base.state == SORT_STATE_FINISHED)
    return;

  int *array = params->base.array;
  int size = params->base.size;

  if (params->base.state == SORT_STATE_IDLE) {
    params->temp_array = malloc(size * sizeof(int));
    params->curr_size = 1;
    params->merge_step = 0;
    params->base.state = SORT_STATE_RUNNING;
  }

  if (params->curr_size < size) {
    if (params->merge_step == 0) {
      params->left = 0;
      params->merge_step = 1;
    }

    if (params->left < size - 1) {
      params->mid = params->left + params->curr_size - 1;
      params->right = (params->mid + params->curr_size < size)
                          ? params->mid + params->curr_size
                          : size - 1;

      if (params->i == 0 && params->j == 0) {
        params->i = params->left;
        params->j = params->mid + 1;
        params->k = params->left;
      }

      if (params->i <= params->mid && params->j <= params->right) {
        params->base.comparisons++;
        if (array[params->i] <= array[params->j]) {
          params->temp_array[params->k] = array[params->i];
          setInsertIndex(&params->base.insert_params, params->k);
          params->i++;
        } else {
          params->temp_array[params->k] = array[params->j];
          setInsertIndex(&params->base.insert_params, params->k);
          params->j++;
        }
        params->k++;
        params->base.inserts++;
      } else if (params->i <= params->mid) {
        params->temp_array[params->k] = array[params->i];
        setInsertIndex(&params->base.insert_params, params->k);
        params->i++;
        params->k++;
        params->base.inserts++;
      } else if (params->j <= params->right) {
        params->temp_array[params->k] = array[params->j];
        setInsertIndex(&params->base.insert_params, params->k);
        params->j++;
        params->k++;
        params->base.inserts++;
      } else {
        for (int x = params->left; x <= params->right; x++) {
          array[x] = params->temp_array[x];
        }
        params->left += 2 * params->curr_size;
        params->i = 0;
        params->j = 0;
      }
    } else {
      params->curr_size *= 2;
      params->merge_step = 0;
    }
  } else {
    free(params->temp_array);
    params->base.state = SORT_STATE_FINISHED;
  }
}