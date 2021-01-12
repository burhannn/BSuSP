#include <stdlib.h>
#include <stdio.h>

#define spaltenA 4
#define spaltenB 5
#define zeilenA 5
#define zeilenB 4

void initializeMatrix(long long* matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			*getAddress(matrix, i, j, cols) = rand() % 100;
		}
	}
}
int main(){

    int64_t matrixA = malloc(spaltenA * zeilenA * sizeof(int64_t));
    int64_t matrixB = malloc(spaltenB * zeilenB * sizeof(int64_t));
    int64_t matrixC = calloc(spaltenB * zeilenB * sizeof(int64_t));

}