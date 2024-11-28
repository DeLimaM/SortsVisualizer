#ifndef SORTVIZ_H
#define SORTVIZ_H

typedef enum { TERMINAL, GUI } interfaceType;
typedef enum { BUBBLE, SELECTION, INSERTION, QUICK, MERGE, NONE } sortType;
typedef enum {
  SORT_STATE_IDLE,
  SORT_STATE_RUNNING,
  SORT_STATE_PAUSED,
  SORT_STATE_FINISHED
} SortState;
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
  int swaps;
  int inserts;
  int comparisons;
  SortState state;
  int i;
  int j;
  int index_min;
  int key;
} sortParams;

void setInterface(interfaceType type);
void doSort(sortType type, int sleep_time);
void setSwapIndex1(swapParams *params, int new_index1);
void setSwapIndex2(swapParams *params, int new_index2);
void setInsertIndex(insertParams *params, int new_index);

#endif // SORTVIZ_H