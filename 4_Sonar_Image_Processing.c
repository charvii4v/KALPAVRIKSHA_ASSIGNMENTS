#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(){

int matrixSize , row ,column ; 
printf("enter the value of Matrix Size");
scanf("%d" , &matrixSize);

if(matrixSize < 2 ||  matrixSize>10){
    printf("input invalid");
    return 1;

}


int **sonar;  //2D matrix allocation

srand(time(0));

// row 
sonar = (int**)malloc(matrixSize * sizeof(int*));

// col
for(row=0 ; row<matrixSize ; row++){
sonar[row] = (int*)malloc(matrixSize * sizeof(int) );
}


// random number generating 
for(row=0 ; row<matrixSize ; row++){
    for(column=0 ; column < matrixSize ; column++){
     *(sonar[row] + column)  = rand() % 256;
    }
}


printf("matrix diplay\n");
 


for(row=0 ; row< matrixSize ; row++){
    for(column=0 ; column<matrixSize ; column++){
     printf("%3d " ,*(sonar[row] + column));
    }
    printf("\n");
}

// rotating 90 degree
for(row = 0; row < matrixSize; row++) {
    for(column = row + 1; column < matrixSize; column++) {
        int temp = *(*(sonar + row) + column);
        *(*(sonar + row) + column) = *(*(sonar + column) + row);
        *(*(sonar + column) + row) = temp;
    }
}

// Reverse rows
for(row = 0; row < matrixSize; row++) {
    int start = 0, end = matrixSize - 1;
    while(start < end) {
        int temp = *(*(sonar + row) + start);
        *(*(sonar + row) + start) = *(*(sonar + row) + end);
        *(*(sonar + row) + end) = temp;
        start++;
        end--;
    }
}

// printing 90 degree rotated matrix

printf("rotated matrix is \n"); 

for(row=0 ; row<matrixSize ; row++){
    for(column=0 ; column<matrixSize ; column++){
        printf("%3d " , *(sonar[row] + column));
    }
    printf("\n");
    
}

// smoothing
for(row=0;row<matrixSize;row++){
    for(column=0;column<matrixSize;column++){
        int sum = 0 , count =0 ;
        for(int sideRow=-1 ; sideRow<=1 ; sideRow++){
            for(int sideColumn=-1 ; sideColumn<=1 ; sideColumn++){
                int neighbourRow = row+sideRow;
                int ncolumn = column + sideColumn;

                if(neighbourRow>=0 && ncolumn>=0 && neighbourRow< matrixSize && ncolumn<matrixSize){
                    sum += *(*(sonar+ neighbourRow) + ncolumn) % 256; // %256 extraxts thr original value 
                    count++;
                }
            }
        }
        *(*(sonar+ row) + column) +=   (sum/count)*256; // upper bits mei conversion 

    }
}

for(row=0 ; row< matrixSize ; row++){
    for(column=0 ; column< matrixSize ; column++){
 *(*(sonar+ row) + column) /= 256;  // new values after smoothing thats y /256 kiya h 
    }
}


printf("smoothed matrix\n");
  for (row = 0; row < matrixSize; row++) {
        for (column = 0; column < matrixSize; column++)
            printf("%3d ", *(*(sonar + row) + column));
        printf("\n");
    }


for(row=0 ; row< matrixSize ; row++){
    free(sonar[row]);
}

free(sonar);

return 0 ;

}


