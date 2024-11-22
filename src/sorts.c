#include "sorts.h"
#include "utils.h"

// ----------------- SORTING ALGORITHMS ----------------
void bubbleSort(int *array, int size, int sleep_time, struct canvas *canvas,
                void (*afterSwap)(int *, int, struct canvas *, int, int, int)) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (array[j] > array[j + 1]) {
        swap(array[j], array[j + 1]);
        afterSwap(array, size, canvas, j, j + 1, sleep_time);
      }
    }
  }
  return;
}

void selectionSort(int *array, int size, int sleep_time, struct canvas *canvas,
                   void (*afterSwap)(int *, int, struct canvas *, int, int,
                                     int)) {}
void insertionSort(int *array, int size, int sleep_time, struct canvas *canvas,
                   void (*afterInsertion)(int, int, struct canvas *, int,
                                          int)) {}
void drunkSort(int *array, int size, int sleep_time, struct canvas *canvas,
               void (*afterSwap)(int *, int, struct canvas *, int, int, int)) {}
void quickSort(int *array, int size, int low, int high, int sleep_time,
               struct canvas *canvas,
               void (*afterSwap)(int *, int, struct canvas *, int, int, int)) {}
void mergeSort(int *array, int size, int low, int high, int sleep_time,
               struct canvas *canvas,
               void (*afterInsertion)(int, int, struct canvas *, int, int)) {}