#include "tasks.h"
#include "os.h"


uint32_t task2_counter;

void task_wait(void){
    while(1);
}

void task1(void){
    P4DIR |= BIT7;
    P4OUT &= ~(BIT7);
    while(1){
        wait(2);
        P4OUT ^= BIT7;
    }
}

void task2(void){
    P1DIR |= BIT0;
    P1OUT &= ~(BIT0);
    while(1){
        wait(4);
        P1OUT ^= BIT0;
    }
}
