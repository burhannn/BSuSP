//
//  automat.c
//  
//
//  Created by Burhan B. on 30.10.20.
//

#include "automat.h"

int calc_coins(int coin, int amount){
    int n = amount/coin;
    printf("Anzahl der %2i-Cent Muenzen: %3i\n", coin, n);
    amount = amount - n*coin;
    return amount;
}

int main(){
    int amount;
    int coins[] = {50, 20, 5, 2, 1};
    printf("Geben Sie den Geldbetrag in Cent ein: ");
    scanf("%d", &amount);
    
    for(int i = 0; i<5; i++){
        amount = calc_coins(coins[i], amount);
    }
    return 0;
}
