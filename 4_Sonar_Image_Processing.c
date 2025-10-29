#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** allocateMatrix(int matrixSize);
void generateRandomMatrix(int** matrix, int matrixSize);
void displayMatrix(int** matrix, int matrixSize, const char* title);
void rotateMatrix90Degrees(int** matrix, int matrixSize);
void smoothMatrix(int** matrix, int matrixSize);
void freeMatrixMemory(int** matrix, int matrixSize);


int main() {
    int matrixSize;

    printf("Enter the size of the matrix (between 2 and 10): ");
    scanf("%d", &matrixSize);

    if (matrixSize < 2 || matrixSize > 10) {
        printf("Invalid matrix size. Please enter a value between 2 and 10.\n");
        return 1;
    }

    srand(time(0)); 

 
    int** sonarMatrix = allocateMatrix(matrixSize);

  
    generateRandomMatrix(sonarMatrix, matrixSize);

    displayMatrix(sonarMatrix, matrixSize, "Original Matrix");

 
    rotateMatrix90Degrees(sonarMatrix, matrixSize);
    displayMatrix(sonarMatrix, matrixSize, "Matrix After 90° Rotation");

    
    smoothMatrix(sonarMatrix, matrixSize);
    displayMatrix(sonarMatrix, matrixSize, "Matrix After Smoothing");

 
    freeMatrixMemory(sonarMatrix, matrixSize);

    return 0;
}


int** allocateMatrix(int matrixSize) {
    int** matrix = (int**)malloc(matrixSize * sizeof(int*));
    if (matrix == NULL) {
        printf("Memory allocation failed for matrix rows.\n");
        exit(1);
    }

    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        matrix[rowIndex] = (int*)malloc(matrixSize * sizeof(int));
        if (matrix[rowIndex] == NULL) {
            printf("Memory allocation failed for matrix columns.\n");
            exit(1);
        }
    }

    return matrix;
}


void generateRandomMatrix(int** matrix, int matrixSize) {
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        for (int columnIndex = 0; columnIndex < matrixSize; columnIndex++) {
            matrix[rowIndex][columnIndex] = rand() % 256;
        }
    }
}


void displayMatrix(int** matrix, int matrixSize, const char* title) {
    printf("\n%s:\n", title);
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        for (int columnIndex = 0; columnIndex < matrixSize; columnIndex++) {
            printf("%3d ", matrix[rowIndex][columnIndex]);
        }
        printf("\n");
    }
}


void rotateMatrix90Degrees(int** matrix, int matrixSize) {
  
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        for (int columnIndex = rowIndex + 1; columnIndex < matrixSize; columnIndex++) {
            int temporaryValue = matrix[rowIndex][columnIndex];
            matrix[rowIndex][columnIndex] = matrix[columnIndex][rowIndex];
            matrix[columnIndex][rowIndex] = temporaryValue;
        }
    }

    
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        int startColumn = 0;
        int endColumn = matrixSize - 1;
        while (startColumn < endColumn) {
            int temporaryValue = matrix[rowIndex][startColumn];
            matrix[rowIndex][startColumn] = matrix[rowIndex][endColumn];
            matrix[rowIndex][endColumn] = temporaryValue;

            startColumn++;
            endColumn--;
        }
    }
}


void smoothMatrix(int** matrix, int matrixSize) {
    
    for (int currentRow = 0; currentRow < matrixSize; currentRow++) {
        for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {

            int sumOfNeighbors = 0;
            int neighborCount = 0;

            for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (int deltaColumn = -1; deltaColumn <= 1; deltaColumn++) {
                    int neighborRow = currentRow + deltaRow;
                    int neighborColumn = currentColumn + deltaColumn;

                    if (neighborRow >= 0 && neighborColumn >= 0 &&
                        neighborRow < matrixSize && neighborColumn < matrixSize) {

                        sumOfNeighbors += matrix[neighborRow][neighborColumn] % 256;
                        neighborCount++;
                    }
                }
            }

            int averageValue = sumOfNeighbors / neighborCount;
            matrix[currentRow][currentColumn] += averageValue * 256; // store in upper bits
        }
    }
    
    for (int currentRow = 0; currentRow < matrixSize; currentRow++) {
        for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
            matrix[currentRow][currentColumn] /= 256;
        }
    }
}


void freeMatrixMemory(int** matrix, int matrixSize) {
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        free(matrix[rowIndex]);
    }
    free(matrix);
}
