#ifndef SORTS_H
#define SORTS_H

#include "utils.h"

typedef enum {
  BUBBLE,
  SELECTION,
  INSERTION,
  DRUNK,
  QUICK,
  MERGE,
  NONE
} sortType;
typedef struct {
  int *array;
  int size;
  int sleep_time;
  sortType type;
  int *prev_array;
} sortParams;

void bubbleSort(sortParams *sortParams);
void selectionSort(sortParams *sortParams);
void insertionSort(sortParams *sortParams);
void drunkSort(sortParams *sortParams);
void quickSort(sortParams *sortParams);
void mergeSort(sortParams *sortParams);

#endif // SORTS_H