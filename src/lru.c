#include <stdio.h>
#include <stdlib.h>

#include "lru.h"

// This is a heap data structure
// Min heap for the datum's timestamp (i.e., last access)

void LRU_init(LRU* lru) {
    lru->size = 7;

    for (int i = 0; i < lru->size; i++) {
        lru->data[i] = NULL;
    }
}

void LRU_insert(LRU* lru, Datum* datum) {
    // check if full.
    if (lru->data[lru->size - 1] == NULL) {
        // add to an empty slot
        int i = lru->size - 1;
        while (lru->data[i] == NULL && i > 0) {
            i--;
        }

        if (lru->data[i] != NULL)
            i++;
        
        lru->data[i] = datum;
        LRU_heapify_up(lru, i);
    } else {
        // eviction
        Datum* head = lru->data[0];
        free(head);
        lru->data[0] = datum;
        LRU_heapify_down(lru);
    }
}

void LRU_heapify_down(LRU* lru) {
    int i = 0;
    int heapified = 0;

    while (!heapified) {
        int time = lru->data[i]->timestamp;

        int left = 2*i + 1;
        if (left >= lru->size || lru->data[left] == NULL)
            break;
        int lefttime = lru->data[left]->timestamp;

        int right = 2*i + 2;
        if (right >= lru->size || lru->data[right] == NULL)
            break;
        int righttime = lru->data[right]->timestamp;

        if (lefttime < time || righttime < time) {
            if (lefttime <= righttime) {
                LRU_swap(lru, i, left);
                i = left;
            } else {
                LRU_swap(lru, i, right);
                i = right;
            }

            continue;
        }

        heapified = 1;
    }
}

void LRU_heapify_up(LRU* lru, int i) {

}

void LRU_swap(LRU* lru, int i, int j) {
    Datum* tmp = lru->data[i];
    lru->data[i] = lru->data[j];
    lru->data[j] = tmp;
}

void LRU_print(LRU* lru) {
    printf("[ ");
    for (int i = 0; i < lru->size; i++) {
        if (lru->data[i] != NULL)
            printf("%s, ", (lru->data[i])->contents);
    }
    printf("]\n");
}