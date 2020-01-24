#include <os.h>

task_t tasks[10];

uint8_t tasks_registered;
uint8_t task_running;

uint16_t *pSched = (uint16_t *) 0x2500;
uint16_t nSched;

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
        asm("mov.w SR,%0" : "=m" (sr_status));

        tasks[tasks_registered].pStack--;

        uint16_t pc_msn = (task_local >> 4) & 0xF000;
        uint16_t pc_lsw = task_local;

        *(tasks[tasks_registered].pStack--) = pc_lsw;
        *(tasks[tasks_registered].pStack) = pc_msn | 0x008;


        // Zerar espaco da pilha de registradores
        for(int i = 1; i <= 24; i++){
            *(--tasks[tasks_registered].pStack) = 0x00;
        }

        tasks_registered++;
    }
}


void clear_memo(void){
    uint16_t *pClear;

    //for(int task = 1; task <= MAX_TASKS; task++){
//        pClear = (STACK_START);
//        for(int i = 0; i <= STACK_END; i++){
//            *(pClear-i) = rand();
//        }
    //}
}

void startRTOS(void){
    WDTCTL = WDTPW | WDTSSEL__ACLK | WDTTMSEL | WDTIS_4; //No 4 a gente ve no 7 fica doidao
    SFRIE1 |= WDTIE;
    P1DIR |= BIT0;

    __enable_interrupt();

//    // Subir o ponteiro da pilha para ele ficar no PC | SR
    tasks[0].pStack += 26;
//
//    // Move o que ta em pStack pro SP, pro MSP saber onde comeca
    asm("movx.a %0,SP" :: "m" (tasks[0].pStack));
//
//    // Precisa colocar o endereco da primeira tarefa?
    asm("pushx.a %0" :: "m" (tasks[0].pTask));

    return;

}

void wait(uint32_t ticks){
    tasks[task_running].wait = ticks;
    while(tasks[task_running].wait);
    return;
}

__attribute__ ((naked))
__attribute__ ((interrupt(WDT_VECTOR)))
void WDT_tick(void){
//  1. Entrada na ISR (SR e PC já estão salvos na pilha da tarefa)

//  2. Salva o contexto, colocando os registros R[4-15] na pilha
    asm("pushm.a  #12,R15");
    asm("movx.a SP,%0" : "=m" (tasks[task_running].pStack));

//  3. Mover o ponteiro da pilha para a pilha do escalonador
    asm("movx.a %0,SP" :: "m" (pSched));

//  4. Executar o escalonador e obter a nova tarefa a ser executada

    for(int i = 0; i < tasks_registered; i++){
        if(tasks[i].wait > 0)
            tasks[i].wait--;
    }
    do{
        task_running = (task_running+1) % tasks_registered;
    }while(tasks[task_running].wait);

//  5. Salvar o ponteiro da pilha do escalonador
    // (Implementacao futura)

//  6. Restaurar o ponteiro da pilha da nova tarefa
    asm("MOV %0,SP" : : "m" (tasks[task_running].pStack));

//  7. Restaura o contexto da nova tarefa
    asm("popm.a #12,R15");

    nSched++;

//  8. Retorna da interrupção (RETI)
    asm("RETI");
}
