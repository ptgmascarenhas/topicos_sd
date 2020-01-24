#include <msp430.h> 
#include "os.h"
#include <stdint.h>
#include <stdlib.h>
#include "tasks.h"

int main(void){
    registerTask(task1, 1);
    registerTask(task2, 1);

    startRTOS();

    while(1){
        continue;
    }

    return 0;
}

