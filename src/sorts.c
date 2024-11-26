#include "sortviz.h"
#include "utils.h"

// ----------------- SORTING ALGORITHMS ----------------
void bubbleSort(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (array[j] > array[j + 1]) {
        swap(array[j], array[j + 1]);
        setSwapIndex1(&params->swap_params, j);
        setSwapIndex2(&params->swap_params, j + 1);
        drawArray(params);
        params->swaps++;
      }
      params->comparisons++;
    }
  }
}

void selectionSort(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  for (int i = 0; i < size - 1; i++) {
    int min_index = i;
    for (int j = i + 1; j < size; j++) {
      if (array[j] < array[min_index]) {
        min_index = j;
      }
    }
    if (min_index != i) {
      swap(array[i], array[min_index]);
      setSwapIndex1(&params->swap_params, i);
      setSwapIndex2(&params->swap_params, min_index);
      drawArray(params);
    }
  }
}

void insertionSort(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  for (int i = 1; i < size; i++) {
    int key = array[i];
    int j = i - 1;

    while (j >= 0 && array[j] > key) {
      array[j + 1] = array[j];
      setInsertIndex(&params->insert_params, j + 1);
      drawArray(params);
      j--;
    }
    array[j + 1] = key;
  }
}

void quickSort(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  void quickSortRecursive(int low, int high) {
    if (low < high) {
      int pivot = array[high];
      int i = low - 1;

      for (int j = low; j <= high - 1; j++) {
        if (array[j] < pivot) {
          i++;
          swap(array[i], array[j]);
          setSwapIndex1(&params->swap_params, i);
          setSwapIndex2(&params->swap_params, j);
          drawArray(params);
        }
      }
      swap(array[i + 1], array[high]);
      setSwapIndex1(&params->swap_params, i + 1);
      setSwapIndex2(&params->swap_params, high);
      drawArray(params);

      int partition_index = i + 1;
      quickSortRecursive(low, partition_index - 1);
      quickSortRecursive(partition_index + 1, high);
    }
  }

  quickSortRecursive(0, size - 1);
}

void mergeSort(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  void merge(int low, int mid, int high) {
    int left_size = mid - low + 1;
    int right_size = high - mid;

    int left[left_size], right[right_size];
    for (int i = 0; i < left_size; i++) {
      left[i] = array[low + i];
    }
    for (int i = 0; i < right_size; i++) {
      right[i] = array[mid + 1 + i];
    }

    int i = 0, j = 0, k = low;
    while (i < left_size && j < right_size) {
      if (left[i] <= right[j]) {
        array[k] = left[i];
        setInsertIndex(&params->insert_params, k);
        drawArray(params);
        i++;
      } else {
        array[k] = right[j];
        setInsertIndex(&params->insert_params, k);
        drawArray(params);
        j++;
      }
      k++;
    }

    while (i < left_size) {
      array[k] = left[i];
      setInsertIndex(&params->insert_params, k);
      drawArray(params);
      i++;
      k++;
    }

    while (j < right_size) {
      array[k] = right[j];
      setInsertIndex(&params->insert_params, k);
      drawArray(params);
      j++;
      k++;
    }
  }

  void mergeSortRecursive(int low, int high) {
    if (low < high) {
      int mid = low + (high - low) / 2;
      mergeSortRecursive(low, mid);
      mergeSortRecursive(mid + 1, high);
      merge(low, mid, high);
      drawArray(params);
    }
  }

  mergeSortRecursive(0, size - 1);
}