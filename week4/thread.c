#include <stdio.h>
#include <pthread.h>

#define COUNT 10000

//starting func for second Thread
void *my_thread(void *param){
    char ch = *((char*)param);

    for(int i = 0;i<COUNT;i++){
        putchar(ch);
    }
}

int main(){

    pthread_t thread_ref; //pthread reference
    char ch1 = '*';
    char ch2 = '.';

    if(pthread_create(&thread_ref, NULL, &my_thread, &ch2)){
        fprintf(stderr,"ERROR: pthread_create failed\n");
        return 1;
    }

    for(int i = 0;i<COUNT;i++){
        putchar(ch1);
    }    

    if(pthread_join(thread_ref, NULL)){
        fprintf(stderr,"ERROR: pthread_join failed\n");
        return 1;
    }

    printf("SUCCESS\n");


}