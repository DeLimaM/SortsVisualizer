#ifndef GUI_H
#define GUI_H

#include "sortviz.h"

#define WINDOW_TITLE "SortViz"
#define REFRESH_RATES_VALUES                                                   \
  { 30, 60, 120, 240, 480 }
#define REFRESH_RATES_COUNT 5
#define DEFAULT_REFRESH_RATE_INDEX 1

#define INDICATORS_ZONE_HEIGHT 50
#define INDICATORS_FONT_SIZE 14
#define INDICATORS_WIDTH 150
#define INDICATORS_MARGIN 10

#define BACKGROUND_COLOR BLACK
#define BAR_COLOR GRAY
#define BAR_SPACING 1

void doSortInGUI(SortType type, int sleep_time, int array_size);

#endif // GUI_H