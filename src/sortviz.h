#ifndef SORTVIZ_H
#define SORTVIZ_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define RESET "\x1b[0m"

#define MAX_STACK_SIZE 32
#define REFRESH_RATE 120

#define random(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))
#define swap(a, b)                                                             \
  do {                                                                         \
    if (a != b) {                                                              \
      int tmp = a;                                                             \
      a = b;                                                                   \
      b = tmp;                                                                 \
    }                                                                          \
  } while (0)

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
  enum { INITIAL_SPLIT, LEFT_HALF, RIGHT_HALF, MERGE_STEP } step;
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

typedef enum {
  PARTITION_START,
  PARTITION_PROCESS,
  HANDLE_LEFT,
  HANDLE_RIGHT
} QuickSortStep;

typedef struct {
  int left;
  int right;
  int pivotIndex;
  int i;
  int j;
  QuickSortStep step;
} QuickState;

typedef struct {
  BaseSortParams base;
  QuickState stack[MAX_STACK_SIZE];
  int stackSize;
} QuickSortParams;

typedef union {
  BaseSortParams base;
  BubbleSortParams bubble;
  SelectionSortParams selection;
  InsertionSortParams insertion;
  MergeSortParams merge;
  QuickSortParams quick;
} SortParamsUnion;

void initBaseParams(BaseSortParams *params, int size, SortType type);
int *createShuffledArray(int size);
void doSort(SortType type, InterfaceType interface, int array_size);
void setSwapIndex(SwapParams *params, int new_index1, int new_index2);
void setInsertIndex(InsertParams *params, int new_index);

#endif // SORTVIZ_H