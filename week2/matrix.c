#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RA 5
#define CA 3
#define RB 3
#define CB 5

void initializeMatrix(long long* matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			*getAddress(matrix, i, j, cols) = rand() % 100;
		}
	}
}
