#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE 100
#define MAX_VALUE ARRAY_SIZE

#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define clear() printf("\033[H\033[J")
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

void drawBar(int value, int index, struct canvas *canvas, int max_value,
             int size) {
  int barWidth = canvas->cols / size;
  int barHeight = value * canvas->lines / max_value;

  for (int i = 0; i < canvas->lines; i++) {
    gotoxy(index * barWidth, canvas->lines - i);
    if (i < barHeight) {
      printf("█");
    } else {
      printf(" ");
    }
  }
}

void drawArray(int *array, int size, struct canvas *canvas) {
  clear();
  hide_cursor();
  for (int i = 0; i < size; i++) {
    drawBar(array[i], i, canvas, MAX_VALUE, size);
  }
  sleep(10);
}

// ----------------- SORTING ALGORITHMS -----------------

void bubbleSort(int *array, int size, struct canvas *canvas) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (array[j] > array[j + 1]) {
        swap(array[j], array[j + 1]);
        drawArray(array, size, canvas);
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
    swap(array[i], array[minIndex]);
    drawArray(array, size, canvas);
  }
}

void shuffle(int *array, int size) {
  for (int i = size - 1; i > 1; i--) {
    int j = random(0, i);
    swap(array[i], array[j]);
  }
}

// ----------------- MAIN -----------------
int main() {
  // get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int lines = w.ws_row;
  int cols = w.ws_col;

  struct canvas canvas;
  initCanvas(&canvas, lines, cols);

  // create and shuffle array
  int array[ARRAY_SIZE];
  for (int i = 0; i < 100; i++) {
    array[i] = i;
  }
  shuffle(array, ARRAY_SIZE);

  selectionSort(array, ARRAY_SIZE, &canvas);

  // reset terminal
  gotoxy(0, canvas.lines + 1);
  printf("\n");
  show_cursor();
  return 0;
}