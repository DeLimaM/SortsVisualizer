#ifndef TERMINAL_H
#define TERMINAL_H

#include "sorts.h"

#define RED "\x1b[31m"
#define WHITE "\x1b[37m"
#define GREEN "\x1b[32m"
#define ORANGE "\x1b[33m"

#define TOP_MARGIN 2
#define COUNTERS_MARGIN 1

#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define clear_window() printf("\033[H\033[J")
#define hide_cursor() printf("\e[?25l")
#define show_cursor() printf("\e[?25h")

void doSortInTerminal(SortType type);

#endif // TERMINAL_H