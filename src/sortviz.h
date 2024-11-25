#ifndef SORTVIZ_H
#define SORTVIZ_H

typedef enum { TERMINAL, GUI } interfaceType;
typedef enum { BUBBLE, SELECTION, INSERTION, QUICK, MERGE, NONE } sortType;
typedef struct {
  int index1;
  int index2;
  int prev_index1;
  int prev_index2;
} swapParams;
typedef struct {
  int index;
  int prev_index;
} insertParams;
typedef struct {
  int *array;
  int size;
  int sleep_time;
  sortType type;
  swapParams swap_params;
  insertParams insert_params;
} sortParams;

void startSort(sortParams *params);
void setInterface(interfaceType type);
void drawArray(sortParams *params);
void doSort(sortType type, int sleep_time);

void setSwapIndex1(swapParams *params, int new_index1);
void setSwapIndex2(swapParams *params, int new_index2);
void setInsertIndex(insertParams *params, int new_index);

#endif // SORTVIZ_H