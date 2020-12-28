#include <stdio.h>
#include <stdlib.h>
void calc_coins(int cent){
    int anz50,anz20,anz5,anz2,anz1;
    while(cent>0){
        if(cent>=50){
            anz50 = cent/50;
            cent -= 50 * anz50;
        }
        else if(cent>=20){
            anz20 = cent/20;
            cent -= 20 * anz20;
        }
        else if(cent>=5){
            anz5 = cent/5;
            cent -= 5 * anz5;
        }
        else if(cent>=2){
            anz2 = cent/2;
            cent -= 2 * anz2;
        }
        else if(cent>=1){
            anz1 = cent/1;
            cent -= 1 * anz1;
        }
    }
    /*for (cent = cent; cent>=50; cent-=50){
        anz50++;
    }
    for (cent; cent>=20; cent-=20){
        anz20++;
    }
    for (cent; cent>=5; cent-=5){
        anz5++;
    }
    for (cent; cent>=2; cent-=2){
        anz2++;
    }
    for (cent; cent>=1; cent-=1){
        anz1++;
    }*/
    printf("Anzahl 50er: %d \n", anz50);
    printf("Anzahl 20er: %d \n", anz20);
    printf("Anzahl 5er: %d \n", anz5);
    printf("Anzahl 2er: %d \n", anz2);
    printf("Anzahl 1er: %d \n", anz1);
    
}
int main(){
    int betrag;
    printf("Bitte geben Sie eine Geldbetrag in Cent ein: ");
    
    if(scanf("%d",&betrag) == 1 && betrag >= 0){
        calc_coins(betrag);
    }
    else
    {
        printf("ERROR\nDer eingegebene Geldbetrag ist kein valider Geldbetrag. ");
        return EXIT_FAILURE;
    }

    return 0;
}