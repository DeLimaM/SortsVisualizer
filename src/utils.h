#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <unistd.h>

#define sleep(ms) usleep(ms * 1000)
#define random(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define clear_window() printf("\033[H\033[J")
#define hide_cursor() printf("\e[?25l")
#define show_cursor() printf("\e[?25h")
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

int *createShuffledArray(int size);
void printUsage();
void signalHandler(int signal);

#endif // UTILS_H