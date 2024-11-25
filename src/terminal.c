#include "terminal.h"
#include "utils.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

// ----------------- SIGNAL HANDLER -----------------
void signalHandlerTerminal(int signal) {
  if (signal == SIGINT) {
    clear_window();
    show_cursor();
    exit(0);
  }
}

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

void drawFullArray(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  for (int i = 0; i < size; i++) {
    drawBar(i, array[i], WHITE);
  }
}

void drawArrayTerminal(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  int swap_index1 = params->swap_params.index1;
  int swap_index2 = params->swap_params.index2;
  int prev_swap_index1 = params->swap_params.prev_index1;
  int prev_swap_index2 = params->swap_params.prev_index2;
  int insert_index = params->insert_params.index;
  int prev_insert_index = params->insert_params.prev_index;

  // draw the previous selected bars
  if (prev_swap_index1 != -1 && prev_swap_index2 != -1) {
    drawBar(prev_swap_index1, array[prev_swap_index1], WHITE);
    drawBar(prev_swap_index2, array[prev_swap_index2], WHITE);
  }
  if (prev_insert_index != -1) {
    drawBar(prev_insert_index, array[prev_insert_index], WHITE);
  }

  sleep(params->sleep_time);

  // draw the current selected bars
  if (swap_index1 != -1 && swap_index2 != -1) {
    drawBar(swap_index1, array[swap_index1], RED);
    drawBar(swap_index2, array[swap_index2], RED);
  }
  if (insert_index != -1) {
    drawBar(insert_index, array[insert_index], GREEN);
  }
}

// ----------------- SORTING -----------------
void doSortInTerminal(sortType type, int sleep_time) {
  signal(SIGINT, signalHandlerTerminal);

  interfaceType interface = TERMINAL;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  lines = w.ws_row;
  cols = w.ws_col;

  int array_size = cols;
  int max_value = lines - TOP_MARGIN;
  int *array = createShuffledArray(array_size);

  sortParams params;
  params.array = array;
  params.size = array_size;
  params.sleep_time = sleep_time;
  params.type = type;
  params.swap_params.index1 = -1;
  params.swap_params.index2 = -1;
  params.swap_params.prev_index1 = -1;
  params.swap_params.prev_index2 = -1;
  params.insert_params.index = -1;
  params.insert_params.prev_index = -1;

  clear_window();
  hide_cursor();

  drawFullArray(&params);
  startSort(&params);
  drawFullArray(&params);

  gotoxy(0, lines);
  printf("\n");
  show_cursor();
}