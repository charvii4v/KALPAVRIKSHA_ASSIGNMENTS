#include<stdio.h>
int main(){
    int numberOfElements;
    scanf("%d" , &numberOfElements );

    int i;
    int array[numberOfElements];
    int freq[numberOfElements];

    for(i=0;i<numberOfElements;i++){
        scanf("%d" , &array[i]);
        freq[i] = -1 ;

    }
    
    for(i=0 ; i <numberOfElements ; i++){
        int count = 1 ; 
        for(int j=i+1 ; j < numberOfElements ; j++){
            if(array[i] == array[j]){
                count++;
                freq[j] = 0 ;
        }
        }
        if(freq[i] != 0){
            freq[i] = count;
        }
        
    }

    for(i=0 ; i <numberOfElements ; i++){
        printf("%d element occurs %d times\n" , array[i] , freq[i]);

    }

    return 0 ; 

}