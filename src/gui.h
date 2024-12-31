#ifndef GUI_H
#define GUI_H

#include "sortviz.h"

#define WINDOW_TITLE "SortViz"

#define INDICATORS_ZONE_HEIGHT 50
#define INDICATORS_FONT_SIZE 14
#define INDICATORS_WIDTH 150
#define INDICATORS_MARGIN 10

#define BACKGROUND_COLOR BLACK
#define BAR_COLOR GRAY
#define BAR_SPACING 1

void doSortInGUI(SortType type, int array_size);

#endif // GUI_H