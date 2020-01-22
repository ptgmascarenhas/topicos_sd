#include "queue.h"

task_t task_error;

uint8_t fifoPut(fifo_t* queue, task_t task){

    if(queue->size == QUEUE_SIZE){  // A fila esta cheia
        return ERROR_QUEUE_FULL;
    }else{ // A fila vai crescer
        queue->fifo[queue->tail] = task;
        queue->tail = (queue->tail+1) % QUEUE_SIZE;
        queue->size += 1;
        return SUCCESS;
    }

}

task_t fifoGet(fifo_t *queue){

    if(queue->size){ // A fila vai diminuir
        queue->head = (queue->head+1) % QUEUE_SIZE;
        queue->size -= 1;
        return queue->fifo[queue->head - 1];
    }else{
        return task_error;
    }
}
