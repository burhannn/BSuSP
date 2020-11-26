#include <limits.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

typedef struct parameter {

    int* array;
    int start;
    int end;
}param;

void arrayFill(int* array, int length){
    for(int i = 0; i < length; i++){
        *(array + i) = 5*i;
    }
}

int get_maximum(int* arr, int length) {
    int max = 0;

   for (int i = 0; i < length; i++) {
       int wert = *(arr+i);
       if(wert > max){
           max = wert;
       }
   }
   return max;
}

/* thread fuktion */
void* get_maximum_parallel(void* data) {
    long *max = malloc(sizeof(long));
    int *arr = ((param*) data)->array;
    int offset = ((param*) data)->start;
    int length = ((param*) data)->end;

    for (int i = offset; i < offset + length; i++) {
        int value = *(arr + i);
        if (value > *max) {
            *max = value;
        }
    }
    free(data);

    return (void*) max;
}

int main(int argc, const char* argv[]){

    if (argc != 3) {
        printf("Usage: maximum <array length> <number of threads>\n");
        exit(EXIT_FAILURE);
    }
    long array_length = strtol(argv[1], NULL, 10);
    if (array_length < 0 || array_length > INT_MAX || array_length & (array_length - 1)) {
        printf("Invalid array length: %ld\n", array_length);
        exit(EXIT_FAILURE);
    }
    printf("array_length=%ld\n", array_length);

    long threads = strtol(argv[2], NULL, 10);
    if (threads < 0 || threads > 16 || threads & (threads - 1)) {
        printf("Invalid number of threads: %ld\n", threads);
        exit(EXIT_FAILURE);
    }

    printf("Initializing...\n");
    srand(time(NULL));

    /* allocate & fill data array */
    int* arr = (int*) malloc(sizeof(int) * array_length);
    if (arr == NULL) {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    arrayFill(arr, array_length);

    /* array for result of each thread */
    int* results = (int*) malloc(sizeof(int) * threads);
    if (results == NULL) {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    /*
       Starting parallel computation
     */
    printf("Starting...\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_t pthreads[threads];
    for (int i = 0; i < threads; i++) {

        param* data = (param*) malloc(sizeof(param));
        data->array = arr;
        data->start = i * (array_length / threads);
        data->end = array_length / threads;

        if (pthread_create(&pthreads[i], NULL, get_maximum_parallel, data) != 0) {
            printf("Could not create threads\n");
            exit(EXIT_FAILURE);
        }
    }

    /*
       Wait for each thread to terminate and store each result in 'results' array
     */
    void* ret_ptr = NULL;
    for (int i = 0; i < threads; i++) {
        pthread_join(pthreads[i], &ret_ptr);
        *(results + i) = * ((long *)ret_ptr);
        free(ret_ptr);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    /* get and print maximum from results */
    printf("Maximum: %d; determined in %ld ms\n", get_maximum(results, threads),
        (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000 / 1000);

    free(arr);
    free(results);
}
