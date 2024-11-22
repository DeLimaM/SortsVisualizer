#include "gui.h"
#include "raylib.h"
#include "utils.h"

void doSortInGUI(sortType type, int sleep_time) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    EndDrawing();
  }

  CloseWindow();

  return;
}