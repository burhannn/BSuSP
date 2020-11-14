#include <stdlib.h>  // Fuer calloc, exit, free, malloc, rand, srand
#include <stdio.h>   // Fuer printf
#include <time.h>    // Fuer time

/* Quelltext der aufgerufenen Funktionen und Makros wie oben angegeben */

int main() {
    if (RA != CB || CA != RB) {
        printf("Ungueltige Dimensionen der Matrizen: Die Zeilenanzahl der Matrix A"
                "muss der Spaltenanzahl der Matrix B gleichen und umgekehrt.\n");
        exit(-1);
    }

    int64_t* matrixA = malloc(RA * CA * sizeof(int64_t));
    int64_t* matrixB = malloc(RB * CB * sizeof(int64_t));
    int64_t* matrixC = calloc(RA * CB, sizeof(int64_t));

    if (matrixA == NULL || matrixB == NULL || matrixC == NULL) {
        printf("ERROR: Allokation fehlgeschlagen.\n");
        exit(-1);
    }

    srand(time(NULL));
    initializeMatrix(matrixA, RA, CA);
    initializeMatrix(matrixB, RB, CB);

    multiplyMatrices(matrixA, matrixB, matrixC, RA, CA, CB);

    printf("Matrix A:\n");
    printMatrix(matrixA, RA, CA);

    printf("\nMatrix B:\n");
    printMatrix(matrixB, RB, CB);

    printf("\nMatrix C (A mal B):\n");
    printMatrix(matrixC, RA, CB);

    free(matrixA);
    free(matrixB);
    free(matrixC);
    return 0;
}