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
  if (index < 0 || value <= 0)
    return;

  int x = index * (bar_width + BAR_SPACING);
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
               INDICATORS_FONT_SIZE, 1, GREEN);
  } else {
    DrawTextEx(font, TextFormat("Swaps: %d", params->base.swaps), right_pos,
               INDICATORS_FONT_SIZE, 1, RED);
  }

  DrawFPS(width - 100, 0);
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
  static int last_size = 0;
  static int last_width = 0;
  static int last_height = 0;

  if (size == last_size && width == last_width && height == last_height) {
    return;
  }

  int total_spacing = (size - 1) * BAR_SPACING;
  bar_width = (width - total_spacing) / size;
  bar_height_multiplier = (float)(height - INDICATORS_ZONE_HEIGHT) / size;

  last_size = size;
  last_width = width;
  last_height = height;
}

void doSortInGUI(SortType type, int sleep_time, int array_size) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN |
                 FLAG_VSYNC_HINT);
  size_t factor = 80;
  InitWindow(factor * 16, factor * 9, WINDOW_TITLE);
  SetExitKey(KEY_NULL);

  font = LoadFontEx("fonts/LexendExa.ttf", INDICATORS_FONT_SIZE, NULL, 0);

  SortParamsUnion params;
  initBaseParams(&params.base, array_size, sleep_time, type);

  // TODO : delete this and replace with an interval-based update in the
  // terminal version
  params.base.sleep_time = 0;

  width = GetScreenWidth();
  int max_size = width / (1 + BAR_SPACING);
  if (params.base.size > max_size) {
    params.base.size = max_size;
    params.base.array = createShuffledArray(params.base.size);

    printf("[Warning] - Array size is too big for the window, resizing to %d\n",
           params.base.size);
  }

  type = params.base.type;

  static const int REFRESH_RATES[] = REFRESH_RATES_VALUES;
  int current_rate_index = DEFAULT_REFRESH_RATE_INDEX;
  double update_interval = 1.0 / REFRESH_RATES[current_rate_index];
  double last_update = GetTime();

  while (!WindowShouldClose()) {
    double current_time = GetTime();

    if (IsKeyPressed(KEY_UP)) {
      current_rate_index = (current_rate_index + 1) % REFRESH_RATES_COUNT;
    } else if (IsKeyPressed(KEY_DOWN)) {
      current_rate_index =
          (current_rate_index - 1 + REFRESH_RATES_COUNT) % REFRESH_RATES_COUNT;
    }
    update_interval = 1.0 / REFRESH_RATES[current_rate_index];

    width = GetScreenWidth();
    height = GetScreenHeight();
    processBarWidthAndHeight(params.base.size, width, height);

    BeginDrawing();

    if (current_time - last_update >= update_interval &&
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

      last_update = current_time;

      ClearBackground(BACKGROUND_COLOR);
      drawFullArrayGUI(&params);
    }

    DrawTextEx(
        font,
        TextFormat("Refresh Rate: %dHz", REFRESH_RATES[current_rate_index]),
        (Vector2){width - 200, height - 30}, INDICATORS_FONT_SIZE, 1, WHITE);

    EndDrawing();
  }

  UnloadFont(font);
  CloseWindow();

  free(params.base.array);
}