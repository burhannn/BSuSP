/* gcc -o aufgabe sched.c matrix.c -lpthread  

./aufgabe 322 131 3
Initializing...
Starting...
Thread 0 beendet: 23 ms
Thread 1 beendet: 29 ms
Thread 2 beendet: 27 ms
Langsamster Thread war 1: 29 ms
Schnellster Thread war 0: 23 ms

*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "matrix.h"

typedef struct eters{
    long long* matrixA;
    long long* matrixB;
    int rows;
    int cols;

}parameter;


void setprio( pthread_t id, int policy, int prio ) {
   struct sched_param param;

   param.sched_priority = prio;

   if((pthread_setschedparam( id, policy, &param)) != 0  ) {
      printf("error: scheduling strategy konnte nicht geaendert werden. Vielleicht kein root?\n");
      pthread_exit((void *)id);
   }
}


/* second thread */
void* my_thread(void* data) {

   long* ret = (long*) malloc(sizeof(long));
   long long* matrixA = ((parameter*) data)->matrixA;
   long long* matrixB = ((parameter*) data)->matrixB;
   int rows = ((parameter*) data)->rows;
   int columns = ((parameter*) data)->cols;

   struct timespec start, end;
   clock_gettime(CLOCK_MONOTONIC, &start);

   long long* matrixC = (long long*)calloc(rows * columns, sizeof(long long));
   if (ret == NULL || matrixC == NULL) {
      printf("Allocation error!");
      exit(EXIT_FAILURE);
   }

   multiplyMatrices(matrixA, matrixB, matrixC, rows, columns, columns);

   clock_gettime(CLOCK_MONOTONIC, &end);
   *ret = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000 / 1000;

   free(matrixC);

    return (void*) ret;
}

int main(int argc, const char* argv[]) {
 
   if(argc != 4){
       printf("ERROR: zu wenig argumente");
   }
   const int zeileAnz = strtol(argv[1], NULL, 10);
   const int spalteAnz = strtol(argv[2], NULL, 10);
   const int anzThreads = strtol(argv[3], NULL, 10);
   if (zeileAnz < 0 || zeileAnz > INT_MAX) {
      printf("Ungueltige Zeilenanzahl: %d\n", zeileAnz);
      exit(EXIT_FAILURE);
   }
   if (spalteAnz < 0 || spalteAnz > INT_MAX) {
      printf("Ungueltige Spaltenanzahl: %d\n", spalteAnz);
      exit(EXIT_FAILURE);
   }
   if (anzThreads < 0 || anzThreads > 16) {
      printf("Ungueltige Anzahl Threads: %d\n", anzThreads);
      exit(EXIT_FAILURE);
   }

   printf("Initializing...\n");


   long long* matrixA = (long long*) malloc(zeileAnz * spalteAnz * sizeof(long long));
   long long* matrixB = (long long*) malloc(zeileAnz * spalteAnz * sizeof(long long));
   parameter* params = (parameter*) malloc(sizeof(parameter));
   if (matrixA == NULL || matrixB == NULL) {
        printf("Allokations error!");
        exit(EXIT_FAILURE);
   }
   srand (time( NULL));
   initializeMatrix(matrixA, zeileAnz, spalteAnz);
   initializeMatrix(matrixB, zeileAnz, spalteAnz);

   printf("Starting...\n");
   params->matrixA = matrixA;
   params->matrixB = matrixB;
   params->rows = zeileAnz;
   params->cols = spalteAnz;
   pthread_t threads[anzThreads];
   
   for (int i = 0; i < anzThreads; i++)
   {
      if (pthread_create(&threads[i], NULL, my_thread, params)!= 0) {
         printf("error: Thread erstellen fehlgeschlagen.\n");
         return EXIT_FAILURE;
      }
      if (i == anzThreads - 1) {
            setprio(threads[i], SCHED_FIFO, 99);
      }
   }
   
   long min = INT_MAX;
   long max = 0;
   int min_index = -1;
   int max_index = -1;
   void* ret_ptr = NULL;
   for (int i = 0; i < anzThreads; i++) {
      pthread_join(threads[i], &ret_ptr);
      long value = *((long*) ret_ptr);
      printf("Thread %d beendet: %ld ms\n", i, value);
      if (value >= max) {
         max = value;
         max_index = i;
      }
      if (value <= min) {
         min = value;
         min_index = i;
      }
      free(ret_ptr);
   }

   printf("Langsamster Thread war %d: %ld ms\n", max_index, max);
   printf("Schnellster Thread war %d: %ld ms\n", min_index, min);
   free(matrixA);
   free(matrixB);
   free(params);
   return EXIT_SUCCESS;
}
