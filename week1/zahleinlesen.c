#include <stdio.h>  // Fuer getchar und printf
#include <stdlib.h> // Fuer exit und malloc

#define ARRAY_LENGTH 11

int main() {

    char* array =  malloc(ARRAY_LENGTH * sizeof(*array));
    if (!array) {
        printf("ERROR: Allokation fehlgeschlagen.\n");
        exit(-1);
    }
    printf("Bitte Zahl eingeben: ");
    int c; 
    int i = 0;
    while((c = getchar())!= EOF && c != '\n'){
        if(c >= 48 && c<=57 && i < ARRAY_LENGTH){
            array[i] = c ;
            
        }
        else{
            printf("ERROR: Keine gültige Zahl eingegeben.");
            exit(-1);
        }
        i++;
    }
    array[i] = 32;
    printf("Ihr eingegebener String : %s", array);
    free(array);
    return 0;
}