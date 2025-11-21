#ifndef QUEUE_H
#define QUEUE_H

#include "node.h"

typedef struct Queue {
    Node* head; 
    Node* tail; 
} Queue;

void initQueue(Queue* q);
Node* createNode(int key, const char* value);
void moveToFront(Queue* q, Node* n);
Node* removeTail(Queue* q);
void addToFront(Queue* q, Node* n);

#endif
