#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <unistd.h>

#define sleep(ms) usleep(ms * 1000)
#define random(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))
#define swap(a, b)                                                             \
  {                                                                            \
    int tmp = a;                                                               \
    a = b;                                                                     \
    b = tmp;                                                                   \
  }
#define swap_ptr(a, b)                                                         \
  {                                                                            \
    int tmp = *a;                                                              \
    *a = *b;                                                                   \
    *b = tmp;                                                                  \
  }

typedef enum {
  BUBBLE,
  SELECTION,
  INSERTION,
  DRUNK,
  QUICK,
  MERGE,
  NONE
} sortType;
typedef enum { TERMINAL, GUI } interfaceType;

struct canvas {
  int lines;
  int cols;
};

int *createShuffledArray(int size);
void printUsage();
void initCanvas(struct canvas *canvas, int lines, int cols);

#endif // UTILS_H