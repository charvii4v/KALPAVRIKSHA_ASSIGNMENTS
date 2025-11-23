#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LRUCache.h"


Node* createNode(int key, char* value) {
    Node* LRU = (Node*)malloc(sizeof(Node));
    LRU->key = key;
    strcpy(LRU->value, value);
    LRU->prev = LRU->next = NULL;
    return LRU;
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
void removeNode(LRUCache* cache, Node* node) {
    if (node->prev)
        node->prev->next = node->next;
    else
        cache->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
}

LRUCache* createCache(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;

    for (int index = 0; index < MAX_HASH; index++)
        cache->hashMap[index] = NULL;

    return cache;
}

char* get(LRUCache* cache, int key) {
    Node* node = cache->hashMap[key];

    if (!node)
        return NULL;

    removeNode(cache, node);
    addToHead(cache, node);

    return node->value;
}

void put(LRUCache* cache, int key, char* value) {
    Node* node = cache->hashMap[key];

    
    if (node) {
        strcpy(node->value, value);
        removeNode(cache, node);
        addToHead(cache, node);
        return;
    }

   
    if (cache->size == cache->capacity) {
        Node* lru = cache->tail;
        cache->hashMap[lru->key] = NULL;
        removeNode(cache, lru);
        free(lru);
        cache->size--;
    }

    Node* newNode = createNode(key, value);
    addToHead(cache, newNode);
    cache->hashMap[key] = newNode;
    cache->size++;
}
