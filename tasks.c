#include "tasks.h"
#include "os.h"

volatile int t1, t2;

void task1(void){
    /*
     * Funcao para piscar led verde
     */
    P4DIR |= BIT7;
    P4OUT &= ~(BIT7);
    while(1){
        wait(400);
        P4OUT ^= BIT7;
    }
}

void task2(void){
    /*
     * Funcao para piscar led vermelho
     */
    P1DIR |= BIT0;
    P1OUT &= ~(BIT0);
    while(1){
        wait(200);
        P1OUT ^= BIT0;
    }
}
