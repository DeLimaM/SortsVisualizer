#include "gui.h"
#include "raylib.h"
#include "sorts.h"
#include "sortviz.h"

void doSortInGUI(SortType type, int sleep_time) {
  InitWindow(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, WINDOW_TITLE);
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  MaximizeWindow();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(DARKGRAY);
    DrawFPS(10, 10);

    EndDrawing();
  }

  CloseWindow();
  return;
}