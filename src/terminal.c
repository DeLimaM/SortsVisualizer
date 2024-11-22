#include "terminal.h"
#include "sorts.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

// ----------------- DRAWING -----------------
// counters
int swaps = 0;
int insertions = 0;

int previous_pivot_index = -1;

void drawCounter() {
  gotoxy(0, 2);
  printf("Swaps: %d, Insertions: %d", swaps, insertions);
}

void drawBar(int value, int index, struct canvas *canvas, const char *color) {
  int max_bar_height = canvas->lines - TOP_MARGIN;
  for (int i = 0; i < max_bar_height - 1; i++) {
    gotoxy(index, max_bar_height - i + TOP_MARGIN);
    if (i < value * max_bar_height / canvas->cols) {
      printf("%s█%s", color, RESET);
    } else {
      printf(" ");
    }
  }
}

void drawArrayAfterSwap(int *array, int size, struct canvas *canvas, int index1,
                        int index2, int sleep_time) {
  hide_cursor();
  if (index1 >= 0 && index1 < size) {
    drawBar(array[index1], index1, canvas, RED);
  }
  if (index2 >= 0 && index2 < size && index2 != index1) {
    drawBar(array[index2], index2, canvas, RED);
  }
  sleep(sleep_time);
  if (index1 >= 0 && index1 < size) {
    drawBar(array[index1], index1, canvas, WHITE);
  }
  if (index2 >= 0 && index2 < size && index2 != index1) {
    drawBar(array[index2], index2, canvas, WHITE);
  }
  swaps++;
  drawCounter();
}

void drawArrayAfterInsertion(int value, int index, struct canvas *canvas,
                             int size, int sleep_time) {
  drawBar(value, index, canvas, RED);
  sleep(sleep_time);
  drawBar(value, index, canvas, WHITE);
  insertions++;
  drawCounter();
}

void drawFullArray(int *array, int size, struct canvas *canvas) {
  for (int i = 0; i < size; i++) {
    drawBar(array[i], i, canvas, WHITE);
  }
}

void doSortInTerminal(sortType type, int sleep_time) {
  // get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int lines = w.ws_row;
  int cols = w.ws_col;

  struct canvas canvas;
  initCanvas(&canvas, lines, cols);

  int array_size = cols;
  int max_value = lines - TOP_MARGIN;

  int *array = createShuffledArray(array_size);

  clear_window();
  hide_cursor();
  drawFullArray(array, array_size, &canvas);

  switch (type) {
  case BUBBLE:
    bubbleSort(array, array_size, sleep_time, &canvas, drawArrayAfterSwap);
    break;
  case SELECTION:
    selectionSort(array, array_size, sleep_time, &canvas, drawArrayAfterSwap);
    break;
  case INSERTION:
    insertionSort(array, array_size, sleep_time, &canvas,
                  drawArrayAfterInsertion);
    break;
  case DRUNK:
    drunkSort(array, array_size, sleep_time, &canvas, drawArrayAfterSwap);
    break;
  case QUICK:
    quickSort(array, array_size, 0, array_size - 1, sleep_time, &canvas,
              drawArrayAfterSwap);
    break;
  case MERGE:
    mergeSort(array, array_size, 0, array_size - 1, sleep_time, &canvas,
              drawArrayAfterInsertion);
    break;
  case NONE:
    break;
  }

  gotoxy(0, canvas.lines + 1);
  printf("\n");
  show_cursor();
}