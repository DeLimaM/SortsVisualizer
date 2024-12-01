#include "gui.h"
#include "raylib.h"
#include "sorts.h"
#include "sortviz.h"
#include <stdio.h>

int width;
int height;
int bar_width;
int max_value;
int bar_height_multiplier;

SortType type;

void drawBarGUI(int index, int value, Color color) {
  if (index < 0)
    return;

  int max_bar_height = height - INDICATORS_ZONE_HEIGHT;
  int bar_height = (value * max_bar_height) / max_value;

  DrawRectangle(index * bar_width, height - bar_height, bar_width, bar_height,
                color);
  DrawRectangle(index * bar_width, INDICATORS_ZONE_HEIGHT, bar_width,
                height - bar_height, BACKGROUND_COLOR);
}

void drawIndicatorsGUI(SortParamsUnion *params) {
  const char *state_str = params->base.state == SORT_STATE_RUNNING ? "RUNNING"
                          : params->base.state == SORT_STATE_FINISHED
                              ? "FINISHED"
                          : params->base.state == SORT_STATE_PAUSED ? "PAUSED"
                                                                    : "IDLE";

  const char *type_str = params->base.type == BUBBLE      ? "BUBBLE"
                         : params->base.type == SELECTION ? "SELECTION"
                         : params->base.type == INSERTION ? "INSERTION"
                         : params->base.type == MERGE     ? "MERGE"
                         : params->base.type == QUICK     ? "QUICK"
                                                          : "NONE";

  DrawText(TextFormat("State: %s", state_str), INDICATORS_MARGIN,
           INDICATORS_MARGIN, INDICATORS_FONT_SIZE, WHITE);
  DrawText(TextFormat("Type: %s", type_str), INDICATORS_MARGIN,
           INDICATORS_MARGIN + INDICATORS_FONT_SIZE, INDICATORS_FONT_SIZE,
           WHITE);
  DrawText(TextFormat("Array size: %d", params->base.size), INDICATORS_MARGIN,
           INDICATORS_MARGIN + 2 * INDICATORS_FONT_SIZE, INDICATORS_FONT_SIZE,
           WHITE);

  DrawText(TextFormat("Comparisons: %d", params->base.comparisons),
           INDICATORS_MARGIN + INDICATORS_WIDTH, INDICATORS_MARGIN,
           INDICATORS_FONT_SIZE, BAR_COLOR);
  DrawText(TextFormat("Swaps: %d", params->base.swaps),
           INDICATORS_MARGIN + INDICATORS_WIDTH,
           INDICATORS_MARGIN + INDICATORS_FONT_SIZE, INDICATORS_FONT_SIZE, RED);
  DrawText(TextFormat("Inserts: %d", params->base.inserts),
           INDICATORS_MARGIN + INDICATORS_WIDTH,
           INDICATORS_MARGIN + 2 * INDICATORS_FONT_SIZE, INDICATORS_FONT_SIZE,
           GREEN);

  DrawFPS(width - 90, 10);
}

void drawFullArrayGUI(SortParamsUnion *params) {
  int *array = params->base.array;
  int size = params->base.size;
  for (int i = 0; i < size; i++) {
    if (i == params->base.swap_params.index1 ||
        i == params->base.swap_params.index2) {
      drawBarGUI(i, array[i], RED);
    } else if (i == params->base.insert_params.index) {
      drawBarGUI(i, array[i], GREEN);
    } else {
      drawBarGUI(i, array[i], BAR_COLOR);
    }
  }
  drawIndicatorsGUI(params);
}

void doSortInGUI(SortType type, int sleep_time, int array_size) {
  InitWindow(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, WINDOW_TITLE);
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetWindowMinSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
  SetTargetFPS(60);

  width = GetScreenWidth();
  height = GetScreenHeight();

  if (array_size > width) {
    printf("WARNING - Array size is too big for the window !\n");
    printf("          Resizing to the window width: %d\n", width);
    array_size = width;
  }

  bar_width = width / array_size > 1 ? width / array_size : 1;
  max_value = array_size + 1;
  bar_height_multiplier = max_value / (height - INDICATORS_ZONE_HEIGHT);

  SortParamsUnion params;
  initBaseParams(&params.base, array_size, sleep_time, type);

  type = params.base.type;
  double lastUpdate = GetTime();

  while (!WindowShouldClose()) {
    double currentTime = GetTime();

    BeginDrawing();

    if (IsWindowResized()) {
      width = GetScreenWidth();
      height = GetScreenHeight();
      bar_width = width / array_size > 1 ? width / array_size : 1;
      max_value = array_size + 1;
      bar_height_multiplier = max_value / (height - INDICATORS_ZONE_HEIGHT);

      printf("Resized to: %d x %d\n", width, height);
      printf("Bar width: %d\n", bar_width);
    }

    if (currentTime - lastUpdate >= 0 / 1000.0 &&
        params.base.state != SORT_STATE_FINISHED) {

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

      lastUpdate = currentTime;

      ClearBackground(BACKGROUND_COLOR);
      drawFullArrayGUI(&params);
    }

    EndDrawing();
  }

  CloseWindow();

  free(params.base.array);
}