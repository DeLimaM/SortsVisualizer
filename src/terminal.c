#include "terminal.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// ----------------- DRAWING -----------------

int lines;
int cols;

void drawBar(int index, int value, const char *color) {
  int max_bar_height = lines - TOP_MARGIN;
  int bar_height = (value * max_bar_height) / cols;
  for (int i = 0; i < max_bar_height; i++) {
    gotoxy(index, max_bar_height - i + TOP_MARGIN);
    if (i < bar_height) {
      printf("%s█%s", color, RESET);
    } else {
      printf(" ");
    }
  }
}

void drawArray(sortParams *params) {
  for (int i = 0; i < params->size; i++) {
    if (params->array[i] != params->prev_array[i]) {
      drawBar(i, params->array[i], RED);
    }
  }
  memcpy(params->prev_array, params->array, params->size * sizeof(int));
  sleep(params->sleep_time);
}

void doSortInTerminal(sortType type, int sleep_time) {

  interfaceType interface = TERMINAL;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  lines = w.ws_row;
  cols = w.ws_col;

  int array_size = cols;
  int max_value = lines - TOP_MARGIN;
  int *array = createShuffledArray(array_size);
  int *prev_array = malloc(array_size * sizeof(int));

  sortParams params;
  params.array = array;
  params.size = array_size;
  params.sleep_time = sleep_time;
  params.type = type;
  params.prev_array = prev_array;

  clear_window();
  hide_cursor();

  startSort(&params);

  gotoxy(0, lines);
  printf("\n");
  show_cursor();
}