#include <stdio.h>

int main() {
    int numberOfElements;
   
    scanf("%d", &numberOfElements);

    int array[numberOfElements];
    
    for (int index = 0; index < numberOfElements; index++) {
        scanf("%d", &array[index]);
    }

    int *outerPointer;
    int *innerPointer;
    int *shiftPointer;
    int *endPointer;

    endPointer = array + numberOfElements; 

    for (outerPointer = array; outerPointer < endPointer; outerPointer++) {

        for (innerPointer = outerPointer + 1; innerPointer < endPointer; innerPointer++) {
   
            if (*innerPointer == *outerPointer) {

                  for (shiftPointer = innerPointer; shiftPointer < endPointer - 1; shiftPointer++) {  
                        *shiftPointer = *(shiftPointer + 1);
                }

                endPointer--;    
                innerPointer--;   
            }
        }
    }

    printf("\nArray after removing duplicate elements:\n");
    for (outerPointer = array; outerPointer < endPointer; outerPointer++) {
        printf("%d ", *outerPointer);
    }
    printf("\n");
    return 0;
}
