#include "tasks.h"
#include "os.h"

volatile int t1, t2;

void task_led_verde(void){
    /*
     * Funcao para piscar led verde
     */
    P4DIR |= BIT7;
    P4OUT &= ~(BIT7);
    while(1){
        wait(200);
        P4OUT ^= BIT7;
    }
}

void task_led_vermelho(void){
    /*
     * Funcao para piscar led vermelho
     */
    P1DIR |= BIT0;
    P1OUT &= ~(BIT0);
    while(1){
        wait(400);
        P1OUT ^= BIT0;
    }
}
