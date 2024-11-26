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

int bar_width;
int bar_margin;

void initBarDimensions(int size) {
  int combined_width_margin = cols / size;
  bar_width = combined_width_margin / 1.5;
  bar_margin = combined_width_margin - bar_width;
}

void drawCountersTerminal(sortParams *params) {
  gotoxy(COUNTERS_MARGIN, COUNTERS_MARGIN);
  printf("Comparisons: %d, Swaps: %d, Inserts: %d", params->comparisons,
         params->swaps, params->inserts);
}

void drawBar(int index, int value, const char *color) {
  int max_bar_height = lines - TOP_MARGIN;
  int bar_height = value;

  // Print the margin spaces before the bar
  for (int x = 0; x < bar_margin; x++) {
    for (int y = 0; y < max_bar_height; y++) {
      gotoxy(index + x, max_bar_height - y);
      printf(" ");
    }
  }

  // Print the bar
  for (int x = 0; x < bar_width; x++) {
    for (int y = 0; y < max_bar_height; y++) {
      gotoxy(index * (bar_width + bar_margin) + bar_margin + x,
             max_bar_height - y);
      if (y < bar_height) {
        printf("%s█%s", color, RESET);
      } else {
        printf(" ");
      }
    }
  }
}

void drawFullArray(sortParams *params) {
  for (int i = 0; i < params->size; i++) {
    drawBar(i, params->array[i], WHITE);
  }

  drawCountersTerminal(params);
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

  drawCountersTerminal(params);
}

// ----------------- SORTING -----------------
void doSortInTerminal(sortType type, int sleep_time) {
  signal(SIGINT, signalHandlerTerminal);

  interfaceType interface = TERMINAL;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  lines = w.ws_row;
  cols = w.ws_col;

  int size = lines;
  initBarDimensions(size);
  int *array = createShuffledArray(size);

  sortParams params;
  params.array = array;
  params.size = size;
  params.sleep_time = sleep_time;
  params.type = type;
  params.swap_params.index1 = -1;
  params.swap_params.index2 = -1;
  params.swap_params.prev_index1 = -1;
  params.swap_params.prev_index2 = -1;
  params.insert_params.index = -1;
  params.insert_params.prev_index = -1;
  params.comparisons = 0;
  params.swaps = 0;
  params.inserts = 0;

  hide_cursor();
  clear_window();

  drawFullArray(&params);
  startSort(&params);
  drawFullArray(&params);

  gotoxy(0, lines);
  printf("\n");
  show_cursor();
}