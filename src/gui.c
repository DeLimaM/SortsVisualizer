#include "gui.h"
#include "raylib.h"
#include "sorts.h"
#include "sortviz.h"
#include <stdio.h>

float width;
float height;
float bar_width = 0;
float bar_spacing = 1;
float bar_height_multiplier;

SortType type;

Font font;
int current_refresh_rate;

void drawBarGUI(int index, int value, Color color) {
  if (index < 0 || value <= 0)
    return;

  int x = index * (bar_width + bar_spacing);
  int bar_height = (int)(value * bar_height_multiplier);
  if (bar_height < 1)
    bar_height = 1;

  Rectangle barRect = {(float)x, (float)(height - bar_height), (float)bar_width,
                       (float)bar_height};
  DrawRectangleRec(barRect, color);
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
  const char *base_text = TextFormat("State: %s\nType: %s\nSize: %d", state_str,
                                     type_str, params->base.size);
  DrawTextEx(font, base_text, (Vector2){INDICATORS_MARGIN, INDICATORS_MARGIN},
             INDICATORS_FONT_SIZE, 1, WHITE);

  Vector2 right_pos = {INDICATORS_MARGIN + INDICATORS_WIDTH, INDICATORS_MARGIN};
  DrawTextEx(font, TextFormat("Comparisons: %d", params->base.comparisons),
             right_pos, INDICATORS_FONT_SIZE, 1, WHITE);
  right_pos.y += INDICATORS_FONT_SIZE;
  if (params->base.type == INSERTION || params->base.type == MERGE) {
    DrawTextEx(font, TextFormat("Inserts: %d", params->base.inserts), right_pos,
               INDICATORS_FONT_SIZE, 1, ORANGE);
  } else {
    DrawTextEx(font, TextFormat("Swaps: %d", params->base.swaps), right_pos,
               INDICATORS_FONT_SIZE, 1, RED);
  }
}

void drawFullArrayGUI(SortParamsUnion *params) {
  int *array = params->base.array;
  int size = params->base.size;
  for (int i = 0; i < size; i++) {
    if (params->base.state == SORT_STATE_FINISHED) {
      drawBarGUI(i, array[i], GREEN);
    } else {
      if (i == params->base.swap_params.index1 ||
          i == params->base.swap_params.index2) {
        drawBarGUI(i, array[i], RED);
      } else if (i == params->base.insert_params.index) {
        drawBarGUI(i, array[i], ORANGE);
      } else {
        drawBarGUI(i, array[i], BAR_COLOR);
      }
    }
  }
  drawIndicatorsGUI(params);
}

void processBarWidthAndHeight(int size, int width, int height) {
  float total_spacing = bar_spacing * size;
  bar_width = (width - total_spacing) / size;

  bar_height_multiplier = (float)(height - INDICATORS_ZONE_HEIGHT) / size;
}

void doSortInGUI(SortType type, int array_size) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN |
                 FLAG_VSYNC_HINT);
  size_t factor = 80;
  InitWindow(factor * 16, factor * 9, WINDOW_TITLE);
  SetExitKey(KEY_NULL);
  SetTargetFPS(60);

  font = LoadFontEx("fonts/LexendExa.ttf", INDICATORS_FONT_SIZE, NULL, 0);

  SortParamsUnion params;
  initBaseParams(&params.base, array_size, type);

  width = GetScreenWidth();
  height = GetScreenHeight();
  int max_size = width / (1 + BAR_SPACING);
  if (params.base.size > max_size) {
    params.base.size = max_size;
    params.base.array = createShuffledArray(params.base.size);

    printf("[Warning] - Array size is too big for the window, resizing to %d\n",
           params.base.size);
  }
  processBarWidthAndHeight(params.base.size, width, height);

  type = params.base.type;

  double update_interval = 1.0 / REFRESH_RATE;
  double last_update = GetTime();

  while (!WindowShouldClose()) {
    double current_time = GetTime();

    width = GetScreenWidth();
    height = GetScreenHeight();
    processBarWidthAndHeight(params.base.size, width, height);

    BeginDrawing();

    if (current_time - last_update >= update_interval) {

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

      last_update = current_time;

      ClearBackground(BACKGROUND_COLOR);
      drawFullArrayGUI(&params);
    }

    EndDrawing();
  }

  UnloadFont(font);
  CloseWindow();

  free(params.base.array);
}