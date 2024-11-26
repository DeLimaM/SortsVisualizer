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
    array[i] = i + 1;
  }
  shuffle(array, size);
  return array;
}
