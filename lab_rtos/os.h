/*
 * Configuracao da memoria de cada TaskN:
 *
 *  0x2801 - T1
 *  0x2850 /
 *  0x2851 - T2
 *  0x2900 /
 *  0x2901 - T3
 *  0x2950 /
 *  0x2951 - T4
 *  0x2A00 /
 *  0x2A01 - T5
 *  0x2A50 /
 *  0x2A51 - T6
 *  0x2B00 /
 *  0x2B01 - T7
 *  0x2B50 /
 *  0x2B51 - T8
 *  0x2C00 /
 *  0x2C01 - T9
 *  0x2C50 /
 *  0x2C51 - T10
 *  0x2D00 /
 *
 * OBS ex: 284c -> PC(4) SR
 *         284e -> PC(16)
 *         2850 -> NADA (No. da trefa)
 *
 * Resumo do que o cafe falou:
 *      Tamo numa tarefaA, ai a gente tem a interrupcao e "tem que salvar o PC e SR atual na pilha da tarefa"
 *      Depois vai colocando tudo dos registradores em cima usando o pStack daquela tarefa
 *      No final o pStack dela ta la em cima
 *
 *      ai vamos para a tarefa B que ta com o pStack la em cima, pegamos e damos POP pros registradores
 *      depois disso pode dar RETI pois essa instrucao ja vai pegar o SR e o PC que tem nos registradores
 *       abaixo do pStack e ja joga pra tarefaB
 *
 */

#ifndef OS
#define OS

#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>

#define STACK_START 0x2800
#define STACK_END 0x2D00
#define MAX_TASKS 10

void registerTask(void* t);
void dispatcher(void);
void clear_memo(void);
void startRTOS(void);
void wait(uint32_t);

typedef struct {
    uint16_t* pStack;  // Ponteiro atual da pilha da tarefa
    uint32_t* pTask;   // Onde esta a tarefa na memoria
    volatile uint32_t wait; // Quantidade de ticks que a tarefa aguarda
} task_t;

//void WDT_tick(void);
#endif
