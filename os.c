#include <os.h>
#include "queue.h"

fifo_t fifo[MAX_PRIORITY_QUEUES+1];     // Filas comuns com prioridade:0 < 1, 2 -> blocked
task_t tasks[MAX_TASKS];                // Vetor de tarefas
int16_t tasks_registered;               // Numero de tarefas ja registradas
int16_t task_running;                   // PID da tarefa em execucao
int8_t quantum_running;                 // Quantum da tarefa em execussao
uint16_t *pSched = (uint16_t *) 0x2500; // Ponteiro da pilha do escalonador

void registerTask(void* func, int8_t priority, uint8_t code, int8_t qt){
    /*  Funcao para registrar uma tarefa no processador
     *  Parametros:
     *      void* func: funcao que vai ser executada pela tarefa, interessante usar funcoes com loop infinito
     *      int8_t priority: prioridade que a tarefa tera, para decidir em qual fila ela deve estar
     *      uint8_t code: codigo que sera passado para possibilitar diferentes tipos de registros
     *          Codigos:
     *              - 0: tarefa comum, registrada por usuario
     *              - 1: tarefa de background, idle, tarefa registrada pelo OS
     *              - 2: demais tarefas registradas pelo OS
     *      int8_t qt: ajuste fino na troca de tarefas e prioridades
     */

    // Verifica se ja se esgotou os espacos de memoria
    if(tasks_registered < MAX_TASKS){

        // Move para o ponteiro da tarefa a posicao da funcao na memoria
        tasks[tasks_registered].pTask = func;

        // Coloca a prioridade na tarefa se for compatvel ou coloca como prioridade 0
        if(priority < MAX_PRIORITY_QUEUES){
            tasks[tasks_registered].priority = priority;
        }else{
            tasks[tasks_registered].priority = 0;
        }

        // Registra quantum a uma tarefa
        tasks[tasks_registered].quantum = qt;
        // Registra wait a uma tarefa
        tasks[tasks_registered].wait = 0;

        // Escolhe o PID com base em regras
        // 0s   -> tarefa background
        // 100s -> tarefas de sistema
        // 500s -> registradas pelo usuario
        // 600s -> declaradas pelo usuario com codigo nao existente
        switch(code){
            case 0:
                tasks[tasks_registered].pid = tasks_registered + 500;
                break;
            case 1:
                tasks[tasks_registered].pid = tasks_registered + 0;
                break;
            case 2:
                tasks[tasks_registered].pid = tasks_registered + 100;
                break;
            default:
                tasks[tasks_registered].pid = tasks_registered + 600;
                break;
        }

        //tasks[tasks_registered].pid = tasks_registered;

        // Variavel auxiliar para guardar o endereco da funcao na memoria
        uint32_t task_local = (uint32_t) func;

        // Aponta para regiao de mem onde ficara a task
        tasks[tasks_registered].pStack = (uint16_t *)(STACK_START + 0x80*(tasks_registered));

        // Registra o PC e o SR inicial
        uint16_t pc_msn = (task_local >> 4) & 0xF000;
        uint16_t pc_lsw = task_local;

        // Anda 1 posicao pra tras
        // Coloca na memoria da maneira descrita no arquivo .h
        tasks[tasks_registered].pStack--;
        *(tasks[tasks_registered].pStack) = pc_lsw;

        tasks[tasks_registered].pStack--;
        *(tasks[tasks_registered].pStack) = pc_msn | 0x008;

        // Zerar espaco da pilha de registradores
        for(int i = 1; i <= 24; i++){
            *(--tasks[tasks_registered].pStack) = 0x00;
        }

        // Declara como sem status especifico
        tasks[tasks_registered].status = 0;

        // Coloca na fila de sua prioridade o seu PID % 100, posicao que foi registrada O(1)
        fifoPut(&fifo[tasks[tasks_registered].priority], tasks[tasks_registered].pid%100);

        // Incrementa em 1 unidade o numero de tarefas registradas
        tasks_registered++;
    }
}

void clear_memo(void){
    /*
     * Funcao para limpar o conteudo da memoria
     */
    uint16_t *pClear = (uint16_t *) 0x2800;
    while(pClear <= (uint16_t *) (STACK_END)){
        *(pClear) = rand();
        pClear++;
    }
}

void task_idle(void){
    /*
     * Tarefa de background do so, escondida do usuario
     */
    while(1);
}

void startRTOS(void){
    /*
     *  Funcao para iniciar o SO
     */

    // Registra a tarefa de background, prioridade 0 e codigo 1
    registerTask(task_idle, 0, 1, 1);

    // Comeca rodando a tarefa background
    task_running = fifoGet(&fifo[0]);

    // Subir o ponteiro da pilha para ele ficar no PC | SR
    tasks[task_running].pStack += 26;

    quantum_running = tasks[task_running].quantum;

    // Move o que ta em pStack pro SP, pro MSP saber onde comeca
    asm("movx.a %0,SP" :: "m" (tasks[task_running].pStack));

    // Precisa colocar o endereco da primeira tarefa?
    asm("pushx.a %0" :: "m" (tasks[task_running].pTask));

    // Habilita as interrupcoes
    WDTCTL = WDTPW | WDTSSEL__ACLK | WDTTMSEL | WDTIS_7 | WDTCNTCL; //No 4 a gente ve no 7 fica doidao
    SFRIE1 |= WDTIE;
    __enable_interrupt();

    asm("RETA");

    // return;
}

void wait(uint32_t ticks){
    /*  Funcao para atribuir um numero de ticks a tarefa
     *  Parametros:
     *      uint32_t ticks: numero de ticks a serem aguardados
     */
    tasks[task_running].wait = ticks;
    // Move tarefa para a fila de bloqueadas
    // Declara o status de blocked
    while(tasks[task_running].wait);
    return;
}

__attribute__ ((naked))
__attribute__ ((interrupt(WDT_VECTOR)))
void WDT_tick(void){

    //if(quantum_running < 0){
        // 1. Entrada na ISR (SR e PC já estão salvos na pilha da tarefa)

        // 2. Salva o contexto, colocando os registros R[4-15] na pilha
        asm("pushm.a  #12,R15");
        asm("movx.a SP,%0" : "=m" (tasks[task_running].pStack));

        // 3. Mover o ponteiro da pilha para a pilha do escalonador
        asm("movx.a %0,SP" :: "m" (pSched));

        // 4. Executar o escalonador e obter a nova tarefa a ser executada

        // Se a tarefa nao foi bloqueada deve ir para a fila
        if(tasks[task_running].wait)
            fifoPut(&fifo[2], tasks[task_running].pid%100);
        else
            fifoPut(&fifo[tasks[task_running].priority], tasks[task_running].pid%100);

        uint8_t size = fifo[2].size;

        // Pega todas as tarefas das bloqueadas
        for(int i = 0; i < size; i++){
            // Pega uma tarefa da fila de bloqueadas
            task_running = fifoGet(&fifo[2]);
            // Diminui seu wait
            tasks[task_running].wait--;
            // Verifica se chegou a 0
            if(tasks[task_running].wait == 0){ // Se chegou a 0 vai pra sua prioridade
                fifoPut(&fifo[tasks[task_running].priority], tasks[task_running].pid%100);
            }else{ // Se ainda nao chegou volta para blocked
                fifoPut(&fifo[2], tasks[task_running].pid%100);
            }
        }

        // Adaptar quando crescer o numero de filas
        if(fifo[1].size){
            task_running = fifoGet(&fifo[1]);
        }else{
            task_running = fifoGet(&fifo[0]);
        }

        quantum_running = tasks[task_running].quantum;

        // 5. Salvar o ponteiro da pilha do escalonador
            // (Implementacao futura)

        // 6. Restaurar o ponteiro da pilha da nova tarefa
        asm("MOV %0,SP" : : "m" (tasks[task_running].pStack));

        // 7. Restaura o contexto da nova tarefa
        asm("popm.a #12,R15");

        // 8. Retorna da interrupção (RETI)
        asm("RETI");
    //}
}
