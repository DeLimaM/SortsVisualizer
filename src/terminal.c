#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define RED "\x1b[31m"
#define WHITE "\x1b[37m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define TOP_MARGIN 2

#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define clear_window() printf("\033[H\033[J")
#define hide_cursor() printf("\e[?25l")
#define show_cursor() printf("\e[?25h")

// ----------------- CANVAS -----------------
struct canvas {
  int lines;
  int cols;
};

void initCanvas(struct canvas *canvas, int lines, int cols) {
  canvas->lines = lines;
  canvas->cols = cols;
}

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

void drawPivot(int *array, int size, struct canvas *canvas, int pivot) {
  if (previous_pivot_index >= 0 && previous_pivot_index < size) {
    drawBar(array[previous_pivot_index], previous_pivot_index, canvas, WHITE);
  }
  drawBar(array[pivot], pivot, canvas, GREEN);
  previous_pivot_index = pivot;
}

void drawFullArray(int *array, int size, struct canvas *canvas) {
  for (int i = 0; i < size; i++) {
    drawBar(array[i], i, canvas, WHITE);
  }
}

// ----------------- SORTING ALGORITHMS ----------------
void bubbleSort(int *array, int size, struct canvas *canvas,
                void (*afterSwap)(int *, int, struct canvas *, int, int, int),
                int sleep_time) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (array[j] > array[j + 1]) {
        swap(array[j], array[j + 1]);
        afterSwap(array, size, canvas, j, j + 1, sleep_time);
      }
    }
  }
}

void selectionSort(int *array, int size, struct canvas *canvas,
                   void (*afterSwap)(int *, int, struct canvas *, int, int,
                                     int),
                   int sleep_time) {
  for (int i = 0; i < size; i++) {
    int minIndex = i;
    for (int j = i + 1; j < size; j++) {
      if (array[j] < array[minIndex]) {
        minIndex = j;
      }
    }
    if (minIndex != i) {
      swap(array[i], array[minIndex]);
      afterSwap(array, size, canvas, i, minIndex, sleep_time);
    }
  }
}

void insertionSort(int *array, int size, struct canvas *canvas,
                   void (*afterInsertion)(int, int, struct canvas *, int, int),
                   int sleep_time) {
  int i = 1;
  while (i < size) {
    int x = array[i];
    int j = i;
    while (j > 0 && array[j - 1] > x) {
      array[j] = array[j - 1];
      afterInsertion(array[j], j, canvas, size, sleep_time);
      j--;
    }
    array[j] = x;
    afterInsertion(array[j], j, canvas, size, sleep_time);
    i++;
  }
}

int partition(int *array, int size, int low, int high, struct canvas *canvas,
              void (*afterSwap)(int *, int, struct canvas *, int, int, int),
              int sleep_time) {
  int pivot = array[high];
  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
      i++;
      swap_ptr(&array[i], &array[j]);
      afterSwap(array, size, canvas, i, j, sleep_time);
    }
  }
  swap_ptr(&array[i + 1], &array[high]);
  afterSwap(array, size, canvas, i + 1, high, sleep_time);
  return i + 1;
}

void quickSort(int *array, int size, int low, int high, struct canvas *canvas,
               void (*afterSwap)(int *, int, struct canvas *, int, int, int),
               int sleep_time) {
  if (low < high && low >= 0) {
    int pi = partition(array, size, low, high, canvas, afterSwap, sleep_time);
    drawPivot(array, size, canvas, pi);
    quickSort(array, size, low, pi - 1, canvas, afterSwap, sleep_time);
    quickSort(array, size, pi + 1, high, canvas, afterSwap, sleep_time);
  }
}

void merge(int *array, int size, int low, int mid, int high,
           struct canvas *canvas,
           void (*afterInsertion)(int, int, struct canvas *, int, int),
           int sleep_time) {
  int n1 = mid - low + 1;
  int n2 = high - mid;

  int L[n1], R[n2];
  for (int i = 0; i < n1; i++) {
    L[i] = array[low + i];
  }
  for (int i = 0; i < n2; i++) {
    R[i] = array[mid + 1 + i];
  }

  int i = 0, j = 0, k = low;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      array[k] = L[i];
      i++;
    } else {
      array[k] = R[j];
      j++;
    }
    afterInsertion(array[k], k, canvas, size, sleep_time);
    k++;
  }

  while (i < n1) {
    array[k] = L[i];
    afterInsertion(array[k], k, canvas, size, sleep_time);
    i++;
    k++;
  }

  while (j < n2) {
    array[k] = R[j];
    afterInsertion(array[k], k, canvas, size, sleep_time);
    j++;
    k++;
  }
}

void mergeSort(int *array, int size, int low, int high, struct canvas *canvas,
               void (*afterInsertion)(int, int, struct canvas *, int, int),
               int sleep_time) {
  if (low < high) {
    int mid = low + (high - low) / 2;
    mergeSort(array, size, low, mid, canvas, afterInsertion, sleep_time);
    mergeSort(array, size, mid + 1, high, canvas, afterInsertion, sleep_time);
    merge(array, size, low, mid, high, canvas, afterInsertion, sleep_time);
  }
}

void drunkSort(int *array, int size, struct canvas *canvas,
               void (*afterSwap)(int *, int, struct canvas *, int, int, int),
               int sleep_time) {
  while (1) {
    int sorted = 1;
    for (int i = 0; i < size - 1; i++) {
      if (array[i] > array[i + 1]) {
        sorted = 0;
        break;
      }
    }
    if (sorted) {
      break;
    }
    for (int i = 0; i < size; i++) {
      int j = random(0, size - 1);
      swap(array[i], array[j]);
      afterSwap(array, size, canvas, i, j, sleep_time);
    }
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
    bubbleSort(array, array_size, &canvas, drawArrayAfterSwap, sleep_time);
    break;
  case SELECTION:
    selectionSort(array, array_size, &canvas, drawArrayAfterSwap, sleep_time);
    break;
  case INSERTION:
    insertionSort(array, array_size, &canvas, drawArrayAfterInsertion,
                  sleep_time);
    break;
  case DRUNK:
    drunkSort(array, array_size, &canvas, drawArrayAfterSwap, sleep_time);
    break;
  case QUICK:
    quickSort(array, array_size, 0, array_size - 1, &canvas, drawArrayAfterSwap,
              sleep_time);
    break;
  case MERGE:
    mergeSort(array, array_size, 0, array_size - 1, &canvas,
              drawArrayAfterInsertion, sleep_time);
    break;
  case NONE:
    break;
  }

  gotoxy(0, canvas.lines + 1);
  printf("\n");
  show_cursor();
}