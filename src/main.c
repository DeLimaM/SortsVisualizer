#include "sortviz.h"
#include <stdio.h>
#include <string.h>

void printUsage() {
  printf("  Usage: sortviz [OPTIONS]\n");
  printf("  Options:\n");
  printf("    --bubble, -b\n");
  printf("    --selection, -s\n");
  printf("    --insertion, -i\n");
  printf("    --quick, -q\n");
  printf("    --merge, -m\n");
  printf("    --gui <do the sort in a gui>\n");
  printf(
      "    --size <size of the array (only accounted for in the gui mode)>\n");
  printf("  Example: sortviz --bubble\n");
  printf("           sortviz -b --gui\n");
}

// ----------------- MAIN -----------------
int main(int argc, char *argv[]) {
  SortType type = NONE;
  InterfaceType interface = TERMINAL;
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
    } else if (strcmp(argv[i], "--quick") == 0 || strcmp(argv[i], "-q") == 0) {
      type = QUICK;
    } else if (strcmp(argv[i], "--merge") == 0 || strcmp(argv[i], "-m") == 0) {
      type = MERGE;
    } else if (strcmp(argv[i], "--size") == 0) {
      if (i + 1 < argc) {
        array_size = atoi(argv[++i]);
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
    doSort(type, interface, array_size);
  } else {
    printf("Error: no sorting algorithm specified\n");
    printUsage();
    return 1;
  }

  return 0;
}