#include "gui.h"
#include "terminal.h"
#include "utils.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>

interfaceType interface = TERMINAL;

// ----------------- MAIN -----------------
int main(int argc, char *argv[]) {

  signal(SIGINT, signalHandler);

  sortType type = NONE;
  int sleep_time = 0;
  int array_size = 100;

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
    } else if (strcmp(argv[i], "--size") == 0) {
      if (i + 1 < argc) {
        int array_size = atoi(argv[++i]);
        if (array_size < 1) {
          printf("Error: size must be greater than 0\n");
          printUsage();
          return 1;
        }
      } else {
        printf("Error: --size requires a value\n");
        printUsage();
        return 1;
      }
    } else if (strcmp(argv[i], "--gui") == 0) {
      interface = GUI;
    } else {
      printf("Unexpected argument: %s\n", argv[i]);
      printUsage();
      return 1;
    }
  }

  if (!(type == NONE)) {
    doSortInTerminal(type, sleep_time);
  } else {
    printf("Error: no sorting algorithm specified\n");
    printUsage();
    return 1;
  }

  return 0;
}