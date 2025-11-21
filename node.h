#ifndef NODE_H
#define NODE_H

typedef struct Node {
    int key;
    char value[100];
    struct Node* prev;
    struct Node* next;
} Node;

#endif
