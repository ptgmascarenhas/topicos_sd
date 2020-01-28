#include "tasks.h"
#include "os.h"

void task1(void){
    /*
     * Funcao para piscar led verde
     */
    P4DIR |= BIT7;
    P4OUT &= ~(BIT7);
    while(1){
        wait(2);
        P4OUT ^= BIT7;
    }
}

void task2(void){
    /*
     * Funcao para piscar led verde
     */
    P1DIR |= BIT0;
    P1OUT &= ~(BIT0);
    while(1){
        wait(4);
        P1OUT ^= BIT0;
    }
}
