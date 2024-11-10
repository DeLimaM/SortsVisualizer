#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define RED "\x1b[31m"
#define WHITE "\x1b[37m"
#define RESET "\x1b[0m"

#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define clear_window() printf("\033[H\033[J")
#define hide_cursor() printf("\e[?25l")
#define show_cursor() printf("\e[?25h")
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
void drawBar(int value, int index, struct canvas *canvas, int size,
             const char *color) {
  for (int i = 0; i < canvas->lines; i++) {
    gotoxy(index, canvas->lines - i);
    if (i < value * canvas->lines / canvas->cols) {
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
    drawBar(array[index1], index1, canvas, size, RED);
  }
  if (index2 >= 0 && index2 < size && index2 != index1) {
    drawBar(array[index2], index2, canvas, size, RED);
  }
  sleep(sleep_time);
  if (index1 >= 0 && index1 < size) {
    drawBar(array[index1], index1, canvas, size, WHITE);
  }
  if (index2 >= 0 && index2 < size && index2 != index1) {
    drawBar(array[index2], index2, canvas, size, WHITE);
  }
}

void drawBarThenResetColor(int value, int index, struct canvas *canvas,
                           int size, int sleep_time) {
  drawBar(value, index, canvas, size, RED);
  sleep(sleep_time);
  drawBar(value, index, canvas, size, WHITE);
}

void drawFullArray(int *array, int size, struct canvas *canvas) {
  for (int i = 0; i < size; i++) {
    drawBar(array[i], i, canvas, size, WHITE);
  }
}

// ----------------- SORTING ALGORITHMS -----------------
void bubbleSort(int *array, int size, struct canvas *canvas) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (array[j] > array[j + 1]) {
        swap(array[j], array[j + 1]);
        drawArrayAfterSwap(array, size, canvas, j, j + 1, 10);
      }
    }
  }
}

void selectionSort(int *array, int size, struct canvas *canvas) {
  for (int i = 0; i < size; i++) {
    int minIndex = i;
    for (int j = i + 1; j < size; j++) {
      if (array[j] < array[minIndex]) {
        minIndex = j;
      }
    }
    if (minIndex != i) {
      swap(array[i], array[minIndex]);
      drawArrayAfterSwap(array, size, canvas, i, minIndex, 50);
    }
  }
}

void insertionSort(int *array, int size, struct canvas *canvas) {
  int i = 1;
  while (i < size) {
    int x = array[i];
    int j = i;
    while (j > 0 && array[j - 1] > x) {
      array[j] = array[j - 1];
      drawBarThenResetColor(array[j], j, canvas, size, 10);
      j--;
    }
    array[j] = x;
    drawBarThenResetColor(array[j], j, canvas, size, 10);
    i++;
  }
}

int partition(int *array, int size, int low, int high, struct canvas *canvas) {
  int pivot = array[high];
  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
      i++;
      swap_ptr(&array[i], &array[j]);
      drawArrayAfterSwap(array, size, canvas, i, j, 10);
    }
  }
  swap_ptr(&array[i + 1], &array[high]);
  drawArrayAfterSwap(array, size, canvas, i + 1, high, 10);
  return i + 1;
}

void quickSort(int *array, int size, int low, int high, struct canvas *canvas) {
  if (low < high && low >= 0) {
    int pi = partition(array, size, low, high, canvas);
    quickSort(array, size, low, pi - 1, canvas);
    quickSort(array, size, pi + 1, high, canvas);
  }
}

void merge(int *array, int size, int low, int mid, int high,
           struct canvas *canvas) {
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
    drawBarThenResetColor(array[k], k, canvas, size, 10);
    k++;
  }

  while (i < n1) {
    array[k] = L[i];
    drawBarThenResetColor(array[k], k, canvas, size, 10);
    i++;
    k++;
  }

  while (j < n2) {
    array[k] = R[j];
    drawBarThenResetColor(array[k], k, canvas, size, 10);
    j++;
    k++;
  }
}

void mergeSort(int *array, int size, int low, int high, struct canvas *canvas) {
  if (low < high) {
    int mid = low + (high - low) / 2;
    mergeSort(array, size, low, mid, canvas);
    mergeSort(array, size, mid + 1, high, canvas);
    merge(array, size, low, mid, high, canvas);
  }
}

void drunkSort(int *array, int size, struct canvas *canvas) {
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
      drawArrayAfterSwap(array, size, canvas, i, j, 10);
    }
  }
}

// ----------------- UTILS -----------------
void shuffle(int *array, int size) {
  srand(time(NULL));
  for (int i = size - 1; i > 0; i--) {
    int j = random(0, i);
    swap(array[i], array[j]);
  }
}

void startSort(int *array, int size, struct canvas *canvas, const char *sort) {
  clear_window();
  hide_cursor();
  drawFullArray(array, size, canvas);
  if (strcmp(sort, "bubble") == 0) {
    bubbleSort(array, size, canvas);
  } else if (strcmp(sort, "selection") == 0) {
    selectionSort(array, size, canvas);
  } else if (strcmp(sort, "insertion") == 0) {
    insertionSort(array, size, canvas);
  } else if (strcmp(sort, "drunk") == 0) {
    drunkSort(array, size, canvas);
  } else if (strcmp(sort, "quick") == 0) {
    quickSort(array, size, 0, size - 1, canvas);
  } else if (strcmp(sort, "merge") == 0) {
    mergeSort(array, size, 0, size - 1, canvas);
  } else {
    clear_window();
    show_cursor();
    printf("Invalid sorting algorithm\n");
    exit(1);
  }
}

// ----------------- SIGNAL HANDLER -----------------
void sigint_handler(int sig) {
  // get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int lines = w.ws_row;
  int cols = w.ws_col;

  struct canvas canvas;
  initCanvas(&canvas, lines, cols);

  if (sig == SIGINT) {
    gotoxy(0, canvas.lines + 1);
    show_cursor();
    exit(0);
  }
}

// set signal handler for SIGINT
void set_signal_action(void) {
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = &sigint_handler;
  sigaction(SIGINT, &act, NULL);
}

// ----------------- MAIN -----------------
int main(int argc, char *argv[]) {
  // get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int lines = w.ws_row;
  int cols = w.ws_col;

  struct canvas canvas;
  initCanvas(&canvas, lines, cols);

  int array_size = cols;
  int max_value = lines;

  // create and shuffle array
  int array[array_size];
  for (int i = 0; i < array_size; i++) {
    array[i] = i;
  }
  shuffle(array, array_size);

  set_signal_action();

  while (--argc > 0) {
    char *arg = *++argv;
    if (strcmp(arg, "-bubble") == 0) {
      startSort(array, array_size, &canvas, "bubble");
    } else if (strcmp(arg, "-selection") == 0) {
      startSort(array, array_size, &canvas, "selection");
    } else if (strcmp(arg, "-insertion") == 0) {
      startSort(array, array_size, &canvas, "insertion");
    } else if (strcmp(arg, "-drunk") == 0) {
      startSort(array, array_size, &canvas, "drunk");
    } else if (strcmp(arg, "-quick") == 0) {
      startSort(array, array_size, &canvas, "quick");
    } else if (strcmp(arg, "-merge") == 0) {
      startSort(array, array_size, &canvas, "merge");
    } else {
      printf("Invalid argument\n");
      printf("  Usage: sortviz [-bubble | -selection | -insertion | -drunk | "
             "-quick | "
             "-merge]\n");
      printf("  Example: sortviz -bubble\n");
      return 1;
    }
  }

  // reset terminal
  gotoxy(0, canvas.lines + 1);
  printf("\n");
  show_cursor();
  return 0;
}