#include "gui.h"
#include "terminal.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

// ----------------- MAIN -----------------
int main(int argc, char *argv[]) {
  sortType type = NONE;
  int sleep_time = 50;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--bubble") == 0 || strcmp(argv[i], "-b") == 0) {
      type = BUBBLE;
    } else if (strcmp(argv[i], "--selection") == 0 ||
               strcmp(argv[i], "-s") == 0) {
      type = SELECTION;
    } else if (strcmp(argv[i], "--insertion") == 0 ||
               strcmp(argv[i], "-i") == 0) {
      type = INSERTION;
    } else if (strcmp(argv[i], "--drunk") == 0 || strcmp(argv[i], "-d") == 0) {
      type = DRUNK;
    } else if (strcmp(argv[i], "--quick") == 0 || strcmp(argv[i], "-q") == 0) {
      type = QUICK;
    } else if (strcmp(argv[i], "--merge") == 0 || strcmp(argv[i], "-m") == 0) {
      type = MERGE;
    } else if (strcmp(argv[i], "--stime") == 0) {
      if (i + 1 < argc) {
        sleep_time = atoi(argv[++i]);
      } else {
        printf("Error: --stime requires a value\n");
        printUsage();
        return 1;
      }
    } else {
      printf("Unexpected argument: %s\n", argv[i]);
      printUsage();
      return 1;
    }
  }

  if (type == NONE) {
    printf("Error: no sorting algorithm specified\n");
    printUsage();
    return 1;
  } else {
    doSortInTerminal(type, sleep_time);
  }
}