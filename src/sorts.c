#include "sorts.h"
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
        setIndex1(&params->swap_params, j);
        setIndex2(&params->swap_params, j + 1);
        drawArray(params);
      }
    }
  }
}

void selectionSort(sortParams *sortParams) {}
void insertionSort(sortParams *sortParams) {}
void drunkSort(sortParams *sortParams) {}
void quickSort(sortParams *sortParams) {}
void mergeSort(sortParams *sortParams) {}
