#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void shuffle(int *array, int size) {
  srand(time(NULL));
  for (int i = size - 1; i > 0; i--) {
    int j = random(0, i);
    swap(array[i], array[j]);
  }
}

void printUsage() {
  printf("  Usage: sortviz [OPTIONS]\n");
  printf("  Options:\n");
  printf("    --bubble, -b\n");
  printf("    --selection, -s\n");
  printf("    --insertion, -i\n");
  printf("    --drunk, -d\n");
  printf("    --quick, -q\n");
  printf("    --merge, -m\n");
  printf("    --stime <value>\n");
  printf("  Example: sortviz --bubble --stime 50\n");
}

int *createShuffledArray(int size) {
  int *array = malloc(size * sizeof(int));
  for (int i = 0; i < size; i++) {
    array[i] = i;
  }
  shuffle(array, size);
  return array;
}