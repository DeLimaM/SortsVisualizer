#include "gui.h"
#include "raylib.h"
#include "sorts.h"
#include "sortviz.h"
#include <stdio.h>

int width;
int height;
int bar_width;
float bar_height_multiplier;

SortType type;

Font font;

void drawBarGUI(int index, int value, Color color) {
  if (index < 0)
    return;

  int bar_height =
      value * bar_height_multiplier > 0 ? value * bar_height_multiplier : 1;
  int x = index * (bar_width + BAR_SPACING);
  int y = height - bar_height;

  DrawRectangle(x, y, bar_width, bar_height, color);
  DrawRectangle(x, INDICATORS_ZONE_HEIGHT, bar_width, y, BACKGROUND_COLOR);
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

  Vector2 position = {INDICATORS_MARGIN, INDICATORS_MARGIN};
  DrawTextEx(font, TextFormat("State: %s", state_str), position,
             INDICATORS_FONT_SIZE, 1, WHITE);
  position.y += INDICATORS_FONT_SIZE;
  DrawTextEx(font, TextFormat("Type: %s", type_str), position,
             INDICATORS_FONT_SIZE, 1, WHITE);
  position.y += INDICATORS_FONT_SIZE;
  DrawTextEx(font, TextFormat("Size: %d", params->base.size), position,
             INDICATORS_FONT_SIZE, 1, WHITE);

  Vector2 statsPos = {INDICATORS_MARGIN + INDICATORS_WIDTH, INDICATORS_MARGIN};
  DrawTextEx(font, TextFormat("Comparisons: %d", params->base.comparisons),
             statsPos, INDICATORS_FONT_SIZE, 1, WHITE);

  statsPos.y += INDICATORS_FONT_SIZE;
  DrawTextEx(font, TextFormat("Swaps: %d", params->base.swaps), statsPos,
             INDICATORS_FONT_SIZE, 1, RED);

  statsPos.y += INDICATORS_FONT_SIZE;
  DrawTextEx(font, TextFormat("Inserts: %d", params->base.inserts), statsPos,
             INDICATORS_FONT_SIZE, 1, GREEN);
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

void processBarWidthAndHeight(int size, int width, int height) {
  int total_spacing = (size - 1) * BAR_SPACING;
  bar_width = (width - total_spacing) / size;
  bar_height_multiplier = (float)(height - INDICATORS_ZONE_HEIGHT) / size;
}

void doSortInGUI(SortType type, int sleep_time, int array_size) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
  size_t factor = 80;
  InitWindow(factor * 16, factor * 9, WINDOW_TITLE);
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  font = LoadFontEx("fonts/LexendExa.ttf", INDICATORS_FONT_SIZE, NULL, 0);

  SortParamsUnion params;
  initBaseParams(&params.base, array_size, sleep_time, type);

  width = GetScreenWidth();

  if (params.base.size > width) {
    params.base.size = width;
    params.base.array = createShuffledArray(params.base.size);

    printf("Array size is too big for the window, resizing to %d\n",
           params.base.size);
  }

  type = params.base.type;
  double lastUpdate = GetTime();

  while (!WindowShouldClose()) {
    double currentTime = GetTime();
    width = GetScreenWidth();
    height = GetScreenHeight();
    processBarWidthAndHeight(params.base.size, width, height);

    BeginDrawing();

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

  UnloadFont(font);
  CloseWindow();

  free(params.base.array);
}