#include <stdio.h>
#include <string.h>
#include "LRUCache.h"

int main() {
    LRUCache* cache = NULL;
    char command[20];


while (1) {
        scanf("%s", command);

        if (!strcmp(command, "createCache")) {
            int size;
            scanf("%d", &size);
            cache = createCache(size);
        }

        else if (!strcmp(command, "put")) {
            int key;
            char value[100];
            scanf("%d %s", &key, value);
            put(cache, key, value);
        }

        else if (!strcmp(command, "get")) {
            int key;
            scanf("%d", &key);
            char* result = get(cache, key);
            if (result)
                printf("%s\n", result);
            else
                printf("NULL\n");
        }

        else if (!strcmp(command, "exit")) {
            break;
        }
    }

    return 0;
}
