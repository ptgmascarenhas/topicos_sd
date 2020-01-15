#include <msp430.h> 
#include "os.h"
#include <stdint.h>
#include <stdlib.h>
#include "tasks.h"

int main(void){
    //clear_memo();

    registerTask(task_wait);
    registerTask(task1);
    registerTask(task2);

    startRTOS();

    while(1){
        continue;
    }

    return 0;
}

