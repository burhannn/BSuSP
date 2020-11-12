//
//  automat.c
//  
//
//  Created by Burhan B. on 30.10.20.
//

#include "automat.h"
#include "calc.h"

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
