#ifndef HEAP_H
#define HEAP_H

#include "player.h"

typedef struct {
    PlayerData* player;
    int team_index;
    PlayerNode* next_node;
} HeapNode;

void swapHeapNodes(HeapNode* a, HeapNode* b);
void maxHeapify(HeapNode heap[], int heapSize, int index);
HeapNode extractMax(HeapNode heap[], int* heapSizePtr);
void insertIntoHeap(HeapNode heap[], int* heapSizePtr, HeapNode new_node);

#endif 
