#ifndef SORTS_H
#define SORTS_H

#include "utils.h"

void bubbleSort(int *array, int size, int sleep_time, struct canvas *canvas,
                void (*afterSwap)(int *, int, struct canvas *, int, int, int));
void selectionSort(int *array, int size, int sleep_time, struct canvas *canvas,
                   void (*afterSwap)(int *, int, struct canvas *, int, int,
                                     int));
void insertionSort(int *array, int size, int sleep_time, struct canvas *canvas,
                   void (*afterInsertion)(int, int, struct canvas *, int, int));
void drunkSort(int *array, int size, int sleep_time, struct canvas *canvas,
               void (*afterSwap)(int *, int, struct canvas *, int, int, int));
void quickSort(int *array, int size, int low, int high, int sleep_time,
               struct canvas *canvas,
               void (*afterSwap)(int *, int, struct canvas *, int, int, int));
void mergeSort(int *array, int size, int low, int high, int sleep_time,
               struct canvas *canvas,
               void (*afterInsertion)(int, int, struct canvas *, int, int));

#endif // SORTS_H