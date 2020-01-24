#include <os.h>
#include "queue.h"

volatile uint32_t problema;
task_t current_task;                        // Tarefa que esta sendo analisada ou rodando no momento
uint8_t tasks_registered;                   // No. de tarefas registradas

uint16_t *pSched = (uint16_t *) 0x4400;     // Ponteiro para a pilha do escalonador
uint32_t nSched;                            // No. de vezes que o escalonador foi chamado

fifo_t fifos[2], fifo_blocked;              // Filas, ordem de prioridade: fifo[1] > fifo[0]
                                            // fifo_blocked guarda as tarefas bloqueadas

void registerTask(void* t, int8_t priority){
    /*  Funcao para registrar uma tarefa no processador
     *  Parametros:
     *      void* t: funcao que vai ser executada pela tarefa, interessante usar funcoes com loop infinito
     *      int8_t priority: prioridade que a tarefa tera, para decidir em qual fila ela deve estar
     */

    task_t task;

    if(tasks_registered <= MAX_TASKS){

        task.pTask = t;            // pTask recebe o endereco da func na mem
        task.priority = priority;  // Seta a prioridade passada no registro
        task.wait = 0;             // Seta a wait passada no registro
        task.quantum = 0;          // Seta a quantum passada no registro


        uint32_t task_local = (uint32_t) t;

        // Aponta para regiao de mem onde ficara a task
        task.pStack = (uint16_t *)(STACK_START + 0x100*(tasks_registered));

        // Coloca na posicao inicial a numero da tarefa
        //*(task.pStack) = tasks_registered + 1;

        // Registra o PC e o SR inicial
        uint16_t sr_status = 0;
        asm("mov.w SR,%0" : "=m" (sr_status));

        task.pStack--;

        uint16_t pc_msn = (task_local >> 4) & 0xF000;
        uint16_t pc_lsw = task_local;

        *(task.pStack--) = pc_lsw;
        *(task.pStack) = pc_msn | 0x008;

        // Zerar espaco da pilha de registradores
        for(int i = 1; i <= 24; i++){
            *(--task.pStack) = 0x00;
        }

        if(priority == 0 || priority == 1){
            fifoPut(&fifos[priority], task);
            tasks_registered++;
        }
    }
}

void clear_memo(void){
    volatile uint16_t random = 0;
    uint16_t *pClear = (uint16_t *) STACK_START;
    while(pClear <= (uint16_t *) (STACK_START-STACK_END)){
        random = rand();
        *(pClear++) = random;
    }
}

void task_idle(void){
    while(1){
        problema++;
    }
}

__attribute__ ((naked))
void startRTOS(void){
    registerTask(task_idle, 0);

    WDTCTL = WDTPW | WDTSSEL__ACLK | WDTTMSEL | WDTIS_7; //No 4 a gente ve no 7 fica doidao
    SFRIE1 |= WDTIE;

    // Subir o ponteiro da pilha para ele ficar no PC | SR
    current_task = fifoGet(&fifos[0]);

    current_task.pStack += 26;

    SFRIFG1 &= ~WDTIFG;

    __enable_interrupt();

    // Move o que ta em pStack pro SP, pro MSP saber onde comeca
    asm("movx.a %0,SP" :: "m" (current_task.pStack));

    // Precisa colocar o endereco da primeira tarefa?
    asm("pushx.a %0" :: "m" (current_task.pTask));

    asm("RETA");
}

void wait(uint32_t ticks){
    current_task.wait = ticks;
    fifoPut(&fifo_blocked, current_task);
    while(current_task.wait);
    return;
}

__attribute__ ((naked))
__attribute__ ((interrupt(WDT_VECTOR)))
void WDT_tick(void){
//  1. Entrada na ISR (SR e PC já estão salvos na pilha da tarefa)

//  2. Salva o contexto, colocando os registros R[4-15] na pilha
    asm("pushm.a  #12,R15");
    asm("movx.a SP,%0" : "=m" (current_task.pStack));

//  3. Mover o ponteiro da pilha para a pilha do escalonador
    asm("movx.a %0,SP" :: "m" (pSched));

//  4. Executar o escalonador e obter a nova tarefa a ser executada
//    scheduler();

    // De modo a seguir filas de prioridades
    if(current_task.wait == 0)
        fifoPut(&fifos[current_task.priority], current_task);

    for(int i = 0; i < fifo_blocked.size; i++){
        current_task = fifoGet(&fifo_blocked);
        if(current_task.wait == 0){
            fifoPut(&fifos[current_task.priority], current_task);
        }else{
            fifoPut(&fifo_blocked, current_task);
        }
    }

    for(int i = 0; i < fifo_blocked.size; i++){
        current_task = fifoGet(&fifo_blocked);
        current_task.wait--;
        fifoPut(&fifo_blocked, current_task);
    }

    if(fifos[1].size){
        current_task = fifoGet(&fifos[1]);
    }else{
        current_task = fifoGet(&fifos[0]);
    }

//  5. Salvar o ponteiro da pilha do escalonador
    // (Implementacao futura)

//  6. Restaurar o ponteiro da pilha da nova tarefa
    asm("MOV %0,SP" :: "m" (current_task.pStack));

//  7. Restaura o contexto da nova tarefa
    asm("popm.a #12,R15");

    nSched++;

//  8. Retorna da interrupção (RETI)
    asm("RETI");
}
