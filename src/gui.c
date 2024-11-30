#include "gui.h"
#include "raylib.h"
#include "sorts.h"
#include "sortviz.h"

int width;

void drawBarGUI(int index, int value, Color color) {
  if (index < 0)
    return;

  int max_bar_height = GetScreenHeight() - 100;
  int bar_height = (value * max_bar_height) / GetScreenWidth();
  DrawRectangle(index, GetScreenHeight() - bar_height, 1, bar_height, color);
  DrawRectangle(index, INDICATORS_ZONE_HEIGHT, 1,
                GetScreenHeight() - bar_height, BACKGROUND_COLOR);
}

void drawIndicatorsGUI(SortParamsUnion *params) {
  const char *stateStr = params->base.state == SORT_STATE_RUNNING ? "RUNNING"
                         : params->base.state == SORT_STATE_FINISHED
                             ? "FINISHED"
                         : params->base.state == SORT_STATE_PAUSED ? "PAUSED"
                                                                   : "IDLE";

  DrawText(TextFormat("State: %s", stateStr), 10, 10, INDICATORS_FONT_SIZE,
           WHITE);
  DrawText(TextFormat("Comparisons: %d", params->base.comparisons), 10,
           10 + INDICATORS_FONT_SIZE, INDICATORS_FONT_SIZE, BAR_COLOR);
  DrawText(TextFormat("Swaps: %d", params->base.swaps), 10,
           10 + 2 * INDICATORS_FONT_SIZE, INDICATORS_FONT_SIZE, RED);
  DrawText(TextFormat("Inserts: %d", params->base.inserts), 10,
           10 + 3 * INDICATORS_FONT_SIZE, INDICATORS_FONT_SIZE, GREEN);

  DrawFPS(width - 90, 10);
}

void drawFullArrayGUI(SortParamsUnion *params) {
  int *array = params->base.array;
  int size = params->base.size;
  for (int i = 0; i < size; i++) {
    drawBarGUI(i, array[i], WHITE);
  }
  drawIndicatorsGUI(params);
}

void doSortInGUI(SortType type, int sleep_time) {
  InitWindow(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, WINDOW_TITLE);
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);

  width = GetScreenWidth();

  SortParamsUnion params;
  initBaseParams(&params.base, width, sleep_time, type);

  double lastUpdate = GetTime();

  while (!WindowShouldClose()) {
    double currentTime = GetTime();

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

  CloseWindow();

  free(params.base.array);
}