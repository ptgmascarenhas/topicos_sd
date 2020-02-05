#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>
#include "os.h"
#include "tasks.h"

int main(void){
    // Fazer o registro das tarefas declaradas em tasks.h
    // registerTask(funcao, fila, codigo, quantum)
    registerTask(task_led_verde, 1, 0, 1);
    registerTask(task_led_vermelho, 1, 0, 1);

    // Iniciar o sistema operacional
    startRTOS();

    // Travar o sistema para que rode as tarefas
    while(1){
        continue;
    }

    return 0;
}

