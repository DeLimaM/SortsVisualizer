#ifndef TERMINAL_H
#define TERMINAL_H

#include "sorts.h"
#include "utils.h"

#define RED "\x1b[31m"
#define WHITE "\x1b[37m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TOP_MARGIN 2

void doSortInTerminal(sortType type, int sleep_time);
void drawArray(sortParams *params);

#endif // TERMINAL_H