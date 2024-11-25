#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shuffle(int *array, int size) {
  srand(time(NULL));
  for (int i = size - 1; i > 0; i--) {
    int j = random(0, i);
    swap(array[i], array[j]);
  }
}

int *createShuffledArray(int size) {
  int *array = malloc(size * sizeof(int));
  for (int i = 0; i < size; i++) {
    array[i] = i;
  }
  shuffle(array, size);
  return array;
}

bool isSorted(int *array, int size) {
  for (int i = 0; i < size - 1; i++) {
    if (array[i] > array[i + 1]) {
      return false;
    }
  }
  return true;
}