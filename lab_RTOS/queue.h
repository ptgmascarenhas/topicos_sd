#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include "os.h"

#define ERROR_QUEUE_FULL -1
#define ERROR_QUEUE_EMPTY -2
#define SUCCESS 0
#define QUEUE_MAX_SIZE 10
#define MAX_PRIORITY_QUEUES 2

typedef struct {
    int16_t fifo[QUEUE_MAX_SIZE]; // Tamanho maximo da fila
    volatile uint8_t head,        // Onde esta iniciando
                     tail,        // Onde esta o fim da fila
                     size;        // Qual e o tamanho da fila
} fifo_t;

int16_t fifoPut(fifo_t*, int16_t);
int16_t fifoGet(fifo_t*);
