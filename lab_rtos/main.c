#include <msp430.h> 
#include "os.h"
#include <stdint.h>
#include <stdlib.h>
#include "tasks.h"

int main(void){
    registerTask(task_idle, 0); // SEMPRE DECLARAR ESSA AQUI
    registerTask(task1, 0);
    registerTask(task2, 0);

    startRTOS();

    while(1){
        continue;
    }

    return 0;
}

