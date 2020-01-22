#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include "os.h"

#define ERROR_QUEUE_FULL 1
#define ERROR_QUEUE_EMPTY -1
#define SUCCESS 0
#define QUEUE_SIZE 5

typedef struct {
    task_t fifo[QUEUE_SIZE];
    uint8_t head, tail, size;
} fifo_t;

uint8_t fifoPut(fifo_t*, task_t);
task_t fifoGet(fifo_t*);
