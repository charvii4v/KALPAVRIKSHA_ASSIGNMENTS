#include<stdio.h>
int main(){

    int numberOfElements;
    scanf("%d" , &numberOfElements);

    int i ;

    int array[numberOfElements];
     for(i=0 ; i < numberOfElements ; i++ ){
        scanf("%d" , &array[i]);
     }

   int isPallindrome = 1 ;
 

   for(i=0 ; i < numberOfElements / 2 ; i++ ){
    if(array[i] != array[numberOfElements-i-1]){
          isPallindrome = 0 ;
          break;

    }
   }
     
    if(isPallindrome){
        printf("Pallindrome");
    }
    else{
        printf("Not Pallindrome");
    }
   

    return 0;

    
}