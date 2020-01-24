#include "queue.h"

uint8_t fifoPut(fifo_t* queue, task_t task){

    if(queue->size == QUEUE_SIZE){  // A fila esta cheia
        return ERROR_QUEUE_FULL;
    }else{ // A fila vai crescer
        queue->tail = (queue->tail+1) % QUEUE_SIZE;
        queue->tasks[queue->tail] = task;
        queue->size += 1;
        return SUCCESS;
    }

}

task_t fifoGet(fifo_t *queue){

    if(queue->size){ // A fila vai diminuir
        queue->head = (queue->head+1) % QUEUE_SIZE;
        queue->size -= 1;
        return queue->tasks[queue->head];

    }
}
