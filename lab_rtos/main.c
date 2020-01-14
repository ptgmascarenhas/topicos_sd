#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>
#include "rtos.h"
#include "tasks.h"

// Testes para verificar o registerTask()
uint32_t *testeA = (uint32_t *)0x000AAAAA;
uint32_t *testeB = (uint32_t *)0x000BBBBB;
uint32_t *testeC = (uint32_t *)0x000CCCCC;

int main(void){

    WDTCTL = WDTPW | WDTSSEL__ACLK | WDTTMSEL | WDTIS_4;
    SFRIE1 |= WDTIE;
    P1DIR |= BIT0;

    __enable_interrupt();

    registerTask(testeA);
    registerTask(testeB);
//    registerTask(testeC);

    while(1){
        continue;
    }

    return 0;
}

