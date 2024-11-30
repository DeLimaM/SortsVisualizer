#ifndef SORTVIZ_H
#define SORTVIZ_H

#define MAX_STACK_SIZE 1000

typedef enum { TERMINAL, GUI } InterfaceType;
typedef enum { BUBBLE, SELECTION, INSERTION, QUICK, MERGE, NONE } SortType;
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
} SwapParams;

typedef struct {
  int index;
  int prev_index;
} InsertParams;

typedef struct {
  int *array;
  int size;
  int sleep_time;
  SortType type;
  SwapParams swap_params;
  InsertParams insert_params;
  int swaps;
  int inserts;
  int comparisons;
  SortState state;
} BaseSortParams;

typedef struct {
  BaseSortParams base;
  int i;
  int j;
} BubbleSortParams;

typedef struct {
  BaseSortParams base;
  int i;
  int j;
  int index_min;
} SelectionSortParams;

typedef struct {
  BaseSortParams base;
  int i;
  int j;
  int key;
} InsertionSortParams;

typedef struct {
  int left;
  int mid;
  int right;
  int step; // 0=initial, 1=left half, 2=right half, 3=merge
} MergeState;

typedef struct {
  BaseSortParams base;
  MergeState stack[MAX_STACK_SIZE];
  int stackSize;
  int *tempArray;
  int mergeIndex;
  int leftIndex;
  int rightIndex;
} MergeSortParams;

typedef union {
  BaseSortParams base;
  BubbleSortParams bubble;
  SelectionSortParams selection;
  InsertionSortParams insertion;
  MergeSortParams merge;
} SortParamsUnion;

void setInterface(InterfaceType type);
void doSort(SortType type, int sleep_time);
void setSwapIndex(SwapParams *params, int new_index1, int new_index2);
void setInsertIndex(InsertParams *params, int new_index);

#endif // SORTVIZ_H