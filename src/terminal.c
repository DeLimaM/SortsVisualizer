#include "terminal.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

void signalHandlerTerminal(int signal) {
  if (signal == SIGINT) {
    clear_window();
    show_cursor();
    exit(0);
  }
}

int lines;
int cols;

void drawBar(int index, int value, const char *color) {
  int max_bar_height = lines - TOP_MARGIN;
  int bar_height = (value * max_bar_height) / cols;
  for (int i = 0; i < max_bar_height; i++) {
    gotoxy(index, max_bar_height - i + TOP_MARGIN);
    if (i < bar_height) {
      printf("%s▊%s", color, RESET);
    } else {
      printf(" ");
    }
  }
}

void drawFullArray(SortParamsUnion *params) {
  int *array = params->base.array;
  int size = params->base.size;
  for (int i = 0; i < size; i++) {
    drawBar(i, array[i], WHITE);
  }
}

void drawIndicators(SortParamsUnion *params) {
  gotoxy(0, 1);
  printf("State: %s  Size: %d  Comparisons: %d  %sSwaps: %d%s  %sInserts: %d%s",
         params->base.state == SORT_STATE_RUNNING    ? "RUNNING"
         : params->base.state == SORT_STATE_FINISHED ? "FINISHED"
         : params->base.state == SORT_STATE_PAUSED   ? "PAUSED"
                                                     : "IDLE",
         params->base.size, params->base.comparisons, RED, params->base.swaps,
         RESET, GREEN, params->base.inserts, RESET);
}

void updateArrayTerminal(SortParamsUnion *params) {
  int *array = params->base.array;

  drawIndicators(params);

  // redraw the swapped bars
  if (params->base.swap_params.prev_index1 >= 0)
    drawBar(params->base.swap_params.prev_index1,
            array[params->base.swap_params.prev_index1], WHITE);

  if (params->base.swap_params.prev_index2 >= 0)
    drawBar(params->base.swap_params.prev_index2,
            array[params->base.swap_params.prev_index2], WHITE);

  if (params->base.swap_params.index1 >= 0)
    drawBar(params->base.swap_params.index1,
            array[params->base.swap_params.index1], RED);

  if (params->base.swap_params.index2 >= 0)
    drawBar(params->base.swap_params.index2,
            array[params->base.swap_params.index2], RED);

  // redraw the inserted bar
  if (params->base.insert_params.prev_index >= 0)
    drawBar(params->base.insert_params.prev_index,
            array[params->base.insert_params.prev_index], WHITE);

  if (params->base.insert_params.index >= 0)
    drawBar(params->base.insert_params.index,
            array[params->base.insert_params.index], GREEN);

  fflush(stdout);
}

void doSortInTerminal(SortType type, int sleep_time) {
  signal(SIGINT, signalHandlerTerminal);

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  lines = w.ws_row;
  cols = w.ws_col;

  SortParamsUnion params;
  params.base.array = createShuffledArray(cols);
  params.base.size = cols;
  params.base.sleep_time = sleep_time;
  params.base.type = type;
  params.base.state = SORT_STATE_IDLE;
  params.base.swaps = 0;
  params.base.inserts = 0;
  params.base.comparisons = 0;

  params.base.swap_params.index1 = -1;
  params.base.swap_params.index2 = -1;
  params.base.swap_params.prev_index1 = -1;
  params.base.swap_params.prev_index2 = -1;

  params.base.insert_params.index = -1;
  params.base.insert_params.prev_index = -1;

  clear_window();
  hide_cursor();
  drawFullArray(&params);

  while (params.base.state != SORT_STATE_FINISHED) {
    switch (params.base.type) {
    case BUBBLE:
      bubbleSortStep(&params.bubble);
      break;
    case SELECTION:
      selectionSortStep(&params.selection);
      break;
    case INSERTION:
      insertionSortStep(&params.insertion);
      break;
    case MERGE:
      mergeSortStep(&params.merge);
      break;
    case QUICK:
      quickSortStep(&params.quick);
      break;
    default:
      break;
    }

    updateArrayTerminal(&params);
    sleep(params.base.sleep_time);
  }

  drawFullArray(&params);
  gotoxy(0, lines);
  printf("\n");
  show_cursor();

  free(params.base.array);
}