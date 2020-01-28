#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>
#include "os.h"
#include "tasks.h"

int main(void){
    // Fazer o registro das tarefas declaradas em tasks.h
    registerTask(task1, 1, 0);
    registerTask(task2, 1, 0);

    // Iniciar o sistema operacional
    startRTOS();

    // Travar o sistema para que rode as tarefas
    while(1){
        continue;
    }

    return 0;
}

