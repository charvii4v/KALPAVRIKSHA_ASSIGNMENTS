#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#define MAX_HASH 10000

typedef struct Node {
    int key;
    char value[100];
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct LRUCache {
    int capacity;
    int size;
    Node* head;
    Node* tail;
    Node* hashMap[MAX_HASH];
} LRUCache;

LRUCache* createCache(int capacity);
char* get(LRUCache* cache, int key);
void put(LRUCache* cache, int key, char* value);

#endif
