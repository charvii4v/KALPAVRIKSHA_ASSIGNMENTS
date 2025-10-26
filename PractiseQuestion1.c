#include<stdio.h>
int main(){
    int numberOfElements;
    scanf("%d"  , &numberOfElements);


    int array[numberOfElements];
    int i;
    for(i=0;i<numberOfElements;i++){
        scanf("%d" , &array[i]);
    }

    int largest = array[0];
    int secondLargest = -100000000 ;

    for(i=1;i<numberOfElements;i++){
        if(array[i] > largest){
            secondLargest = largest;
            largest = array[i];
        }
        else{
           if(array[i]< largest && array[i] > secondLargest){
              secondLargest = array[i];
           }
        }
    }
    
    printf("the Second Largest number is %d\n" , secondLargest);

    int smallest = array[0] ;
    int secondSmallest = 100000000 ;
    
    
    for(i=1;i<numberOfElements;i++){
        if(array[i] < smallest){
            secondSmallest = smallest; 
            smallest = array[i];
        }
        else{
            if(array[i] > smallest && array[i] < secondSmallest){
                secondSmallest = array[i];
            }
        }
    }

    printf("\nthe Second Smallest number is %d\n" , secondSmallest);

    return 0;


}
