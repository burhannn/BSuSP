#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
//#include <sys/wait.h>

#include "matrix.h"
#define RA 500
#define RB 700
#define CB 500
#define CA 700


int main() {
    if (RA != CB || CA != RB) {
        printf("Ungueltige Dimensionen der Matrizen: Die Zeilenanzahl der Matrix A"
                "muss der Spaltenanzahl der Matrix B gleichen und umgekehrt.\n");
        exit(-1);
    }

    pid_t p;
    p = fork();
    int seconds = 0;
    int status = -1;
    if( p == (pid_t)0 ){ // child Prozess

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
    }
    else if (p != (pid_t)-1){
        //Parent
        printf ("Parent: PID: %d, waiting for child ...\n", getpid());
        
        while (status == -1)
        {
            waitpid(p, &status, WNOHANG);
            printf("Wartezeit: %d\n",seconds);
            seconds++;
            sleep(1);
        }
        printf("Parent: done.\n");
        printf("Status: %d\n", status);

    }
    else
    {
        //ERROR
    }
    
    return 0;
}