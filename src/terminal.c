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

void drawArrayTerminal(sortParams *params) {
  int *array = params->array;
  int size = params->size;

  // draw the previous swapped bars
  drawBar(params->swap_params.prev_index1,
          array[params->swap_params.prev_index1], WHITE);
  drawBar(params->swap_params.prev_index2,
          array[params->swap_params.prev_index2], WHITE);

  sleep(params->sleep_time);

  // draw the swapped bars
  drawBar(params->swap_params.index1, array[params->swap_params.index1], RED);
  drawBar(params->swap_params.index2, array[params->swap_params.index2], RED);
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

  clear_window();
  hide_cursor();

  startSort(&params);

  gotoxy(0, lines);
  printf("\n");
  show_cursor();
}