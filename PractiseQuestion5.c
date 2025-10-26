#include <stdio.h>

int main() {
    int numberOfElements;

    scanf("%d", &numberOfElements);
    
    int sourceArray[numberOfElements];
    int destinationArray[numberOfElements];

    int *sourcePointer;
    int *destinationPointer;
 
    printf("Enter %d elements:\n", numberOfElements);
    for (int i = 0; i < numberOfElements; i++) {
        scanf("%d", &sourceArray[i]);
    }

    sourcePointer = sourceArray;
    destinationPointer = destinationArray;
 
    for (int i = 0; i < numberOfElements; i++) {
        *(destinationPointer + i) = *(sourcePointer + i);
    }

    printf("Elements copied to destination array:\n");
    for (int i = 0; i < numberOfElements; i++) {
        printf("%d ", destinationArray[i]);
    }

    return 0;
}
