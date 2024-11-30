#ifndef GUI_H
#define GUI_H

#include "sortviz.h"

#define WINDOW_TITLE "SortViz"
#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600

#define INDICATORS_ZONE_HEIGHT 50
#define INDICATORS_FONT_SIZE 10
#define INDICATORS_WIDTH 125
#define INDICATORS_MARGIN 10

#define BACKGROUND_COLOR DARKGRAY
#define BAR_COLOR WHITE

void doSortInGUI(SortType type, int sleep_time, int array_size);

#endif // GUI_H