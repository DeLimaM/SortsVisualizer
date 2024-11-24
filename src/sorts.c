#include "sorts.h"
#include "gui.h"
#include "terminal.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>

// ----------------- SORTING ALGORITHMS ----------------
void bubbleSort(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (array[j] > array[j + 1]) {
        swap(array[j], array[j + 1]);
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

extern interfaceType interface;
