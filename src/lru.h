#ifndef LRU_H
#define LRU_H

#include "datum.h"

typedef struct {
    Datum* data[7];
    int size;
} LRU;

void LRU_init(LRU* lru);
void LRU_insert(LRU* lru, Datum* datum);
void LRU_heapify_down(LRU* lru);
void LRU_heapify_up(LRU* lru, int i);
void LRU_swap(LRU* lru, int i, int j);
void LRU_print(LRU* lru);

#endif