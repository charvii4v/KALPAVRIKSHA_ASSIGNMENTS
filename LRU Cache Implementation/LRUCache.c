#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LRUCache.h"


Node* createNode(int key, char* value) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->key = key;
    strcpy(n->value, value);
    n->prev = n->next = NULL;
    return n;
}

void addToHead(LRUCache* cache, Node* node) {
    node->next = cache->head;
    node->prev = NULL;

    if (cache->head != NULL)
        cache->head->prev = node;

    cache->head = node;

    if (cache->tail == NULL)
        cache->tail = node;
}
