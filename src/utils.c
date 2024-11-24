#include "utils.h"
#include "sorts.h"
#include <signal.h>
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

void signalHandler(int signal) {
  if (signal == SIGINT) {
    show_cursor();
    clear_window();
    exit(0);
  }
}

void startSort(sortParams *params) {
  switch (params->type) {
  case BUBBLE:
    bubbleSort(&params);
    break;
  case SELECTION:
    selectionSort(&params);
    break;
  case INSERTION:
    insertionSort(&params);
    break;
  case DRUNK:
    drunkSort(&params);
    break;
  case QUICK:
    quickSort(&params);
    break;
  case MERGE:
    mergeSort(&params);
    break;
  }
}