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

// ----------------- CANVAS -----------------
struct canvas {
  int lines;
  int cols;
};

void initCanvas(struct canvas *canvas, int lines, int cols) {
  canvas->lines = lines;
  canvas->cols = cols;
}

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

void shuffle(int *array, int size) {
  for (int i = size - 1; i > 1; i--) {
    int j = random(0, i);
    swap(array[i], array[j]);
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
    printf("\n");
    show_cursor();
    exit(0);
  }
}

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

  clear_window();
  hide_cursor();
  set_signal_action();
  drawFullArray(array, array_size, &canvas);

  while (--argc > 0) {
    char *arg = *++argv;
    if (strcmp(arg, "-b") == 0) {
      bubbleSort(array, array_size, &canvas);
    } else if (strcmp(arg, "-s") == 0) {
      selectionSort(array, array_size, &canvas);
    }
  }

  // reset terminal
  gotoxy(0, canvas.lines + 1);
  printf("\n");
  show_cursor();
  return 0;
}