#ifndef GUI_H
#define GUI_H

#include "sortviz.h"

#define WINDOW_TITLE "SortViz"
#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600
#define WINDOW_MIN_WIDTH WINDOW_DEFAULT_WIDTH
#define WINDOW_MIN_HEIGHT WINDOW_DEFAULT_HEIGHT
#define DEFAULT_MONITOR_WIDTH 1920

#define INDICATORS_ZONE_HEIGHT 50
#define INDICATORS_FONT_SIZE 10
#define INDICATORS_WIDTH 125
#define INDICATORS_MARGIN 10

#define BACKGROUND_COLOR BLACK
#define BAR_COLOR GRAY

void doSortInGUI(SortType type, int sleep_time, int array_size);

#endif // GUI_H