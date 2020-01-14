#include "rtos.h"
// Recebe o nome da funcao como parametro e pega o primeiro endereco

void registerTask(void* t){

    if(tasks_registered <= MAX_TASKS){
        tasks[tasks_registered].pTask = t;      // pTask recebe o endereco da func na mem

        uint32_t task_local = (uint32_t) t;

        // Aponta para regiao de mem onde ficara a task
        tasks[tasks_registered].pStack = (STACK_START + 0x50*(tasks_registered+1));

        // Coloca na posicao inicial a numero da tarefa
        *(tasks[tasks_registered].pStack) = tasks_registered + 1;

        // Registra o PC e o SR inicial
        uint16_t sr_status = 0;
        asm("mov.w SR,%0" : "m=" (sr_status));

        tasks[tasks_registered].pStack--;

        uint16_t pc_msn = (task_local >> 4) & 0xF000;
        uint16_t pc_lsw = task_local;

        *(tasks[tasks_registered].pStack--) = pc_lsw;
        *(tasks[tasks_registered].pStack) = pc_msn | sr_status;


        // Zerar espaco da pilha de registradores
        for(int i = 1; i <= 24; i++){
            *(--tasks[tasks_registered].pStack) = 0x00;
        }

        tasks_registered++;
    }
}

void dispatcher(void){
//  1. Entrada na ISR (SR e PC já estão salvos na pilha da tarefa)

//  2. Salva o contexto, colocando os registros R[4-15] na pilha

//  3. Mover o ponteiro da pilha para a pilha do escalonador

//  4. Executar o escalonador e obter a nova tarefa a ser executada

//  5. Salvar o ponteiro da pilha do escalonador

//  6. Restaurar o ponteiro da pilha da nova tarefa

//  7. Restaura o contexto da nova tarefa

//  8. Retorna da interrupção (RETI)
    asm("RETI");
}

void WDT_tick(void){
//    P1OUT |= BIT0;
//    dispatcher();
//    P1OUT &= ~(BIT0);
    P1OUT ^= BIT0;
}
