#include <stdio.h>
#include <stdlib.h>
int main()
{
    char* zahlen = malloc(10 * sizeof(zahlen));
    if(zahlen == NULL){
        printf("Speicherallokation fehlgeschlagen");
        exit(-1);
    }
    int zahl, i = 0;
    printf("Bitte eine natürliche Zahl eingeben: \n");
    
    while((zahl = getchar()) >= 48 && zahl <= 57 && zahl != EOF){
        zahlen[i] = zahl;
        i++;
    }
    zahlen[i] = '\0';
    printf("Zahl: %s\n", zahlen);
    
    return EXIT_SUCCESS;
}
