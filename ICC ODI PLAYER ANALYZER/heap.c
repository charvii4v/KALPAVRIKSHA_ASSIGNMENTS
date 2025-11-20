#include "heap.h"
#include <stdio.h>

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void maxHeapify(HeapNode heap[], int heapSize, int index) {
    int largestIndex = index;
    int leftIndex = 2 * index + 1;
    int rightIndex = 2 * index + 2;

    if (leftIndex < heapSize && heap[leftIndex].player->performanceIndex > heap[largestIndex].player->performanceIndex) {
        largestIndex = leftIndex;
    }
    if (rightIndex < heapSize && heap[rightIndex].player->performanceIndex > heap[largestIndex].player->performanceIndex) {
        largestIndex = rightIndex;
    }

    if (largestIndex != index) {
        swapHeapNodes(&heap[index], &heap[largestIndex]);
        maxHeapify(heap, heapSize, largestIndex);
    }
}

HeapNode extractMax(HeapNode heap[], int* heapSizePtr) {
    if (*heapSizePtr <= 0) return (HeapNode){NULL, -1, NULL};

    HeapNode maxNode = heap[0];
    heap[0] = heap[*heapSizePtr - 1];
    (*heapSizePtr)--;
    maxHeapify(heap, *heapSizePtr, 0);
    return maxNode;
}

void insertIntoHeap(HeapNode heap[], int* heapSizePtr, HeapNode new_node) {
    if (*heapSizePtr == 10) {
        printf("Heap is full\n");
        return;
    }

    (*heapSizePtr)++;
    int currentIndex = *heapSizePtr - 1;
    heap[currentIndex] = new_node;

    while (currentIndex != 0 && heap[(currentIndex - 1) / 2].player->performanceIndex < heap[currentIndex].player->performanceIndex) {
        swapHeapNodes(&heap[currentIndex], &heap[(currentIndex - 1) / 2]);
        currentIndex = (currentIndex - 1) / 2;
    }
}
