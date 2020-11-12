#include <stdio.h> 
#include "calc.h"

int calc_coins(int coin, int amount){
    int n = amount/coin;
    printf("Anzahl der %2i-Cent Muenzen: %3i\n", coin, n);
    amount = amount - n*coin;
    return amount;
}