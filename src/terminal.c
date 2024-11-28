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

void drawIndicators(sortParams *params) {
  gotoxy(0, 1);
  printf("State: %s  Size: %d  Comparisons: %d  %sSwaps: %d%s  %sInserts: %d%s",
         params->state == SORT_STATE_RUNNING    ? "RUNNING"
         : params->state == SORT_STATE_FINISHED ? "FINISHED"
         : params->state == SORT_STATE_PAUSED   ? "PAUSED"
                                                : "IDLE",
         params->size, params->comparisons, RED, params->swaps, RESET, GREEN,
         params->inserts, RESET);
}

void updateArrayTerminal(sortParams *params) {
  int *array = params->array;

  drawIndicators(params);

  // redraw the swapped bars
  if (params->swap_params.prev_index1 >= 0)
    drawBar(params->swap_params.prev_index1,
            array[params->swap_params.prev_index1], WHITE);

  if (params->swap_params.prev_index2 >= 0)
    drawBar(params->swap_params.prev_index2,
            array[params->swap_params.prev_index2], WHITE);

  if (params->swap_params.index1 >= 0)
    drawBar(params->swap_params.index1, array[params->swap_params.index1], RED);

  if (params->swap_params.index2 >= 0)
    drawBar(params->swap_params.index2, array[params->swap_params.index2], RED);

  // redraw the inserted bar
  if (params->insert_params.prev_index >= 0)
    drawBar(params->insert_params.prev_index,
            array[params->insert_params.prev_index], WHITE);

  if (params->insert_params.index >= 0)
    drawBar(params->insert_params.index, array[params->insert_params.index],
            GREEN);

  fflush(stdout);
}

// ----------------- SORTING -----------------
void doSortInTerminal(sortType type, int sleep_time) {
  signal(SIGINT, signalHandlerTerminal);

  interfaceType interface = TERMINAL;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  lines = w.ws_row;
  cols = w.ws_col;

  sortParams params;
  params.array = createShuffledArray(cols);
  params.size = cols;
  params.sleep_time = sleep_time;
  params.type = type;
  params.swap_params.index1 = -1;
  params.swap_params.index2 = -1;
  params.swap_params.prev_index1 = -1;
  params.swap_params.prev_index2 = -1;
  params.insert_params.index = -1;
  params.insert_params.prev_index = -1;
  params.swaps = 0;
  params.inserts = 0;
  params.comparisons = 0;
  params.state = SORT_STATE_IDLE;
  params.i = 0;
  params.j = 0;

  clear_window();
  hide_cursor();
  drawFullArray(&params);

  while (params.state != SORT_STATE_FINISHED) {
    switch (params.type) {
    case BUBBLE:
      bubbleSortStep(&params);
      break;
    default:
      break;
    }

    updateArrayTerminal(&params);
    sleep(params.sleep_time);
  }

  drawFullArray(&params);
  gotoxy(0, lines);
  printf("\n");
  show_cursor();
}