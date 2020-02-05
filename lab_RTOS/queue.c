#include "queue.h"

int16_t fifoPut(fifo_t* queue, int16_t id){
    /*  Funcao para incluir elementos em uma fila
     *  Parametros:
     *      fifo_t* queue: fila destinho, usar & antes do nome
     *      int16_t id: pid do processo a ser enviado para a fila
     */

    if(queue->size == QUEUE_MAX_SIZE){  // A fila esta cheia
        return ERROR_QUEUE_FULL;
    }else{ // A fila vai crescer
        queue->fifo[queue->tail] = id;
        queue->tail = (queue->tail+1) % QUEUE_MAX_SIZE;
        queue->size += 1;
        return SUCCESS;
    }

}

int16_t fifoGet(fifo_t *queue){
    /*  Funcao para retirar elementos em uma fila
     *  Parametros:
     *      fifo_t* queue: fila destinho, usar & antes do nome
     */

    if(queue->size){ // A fila vai diminuir
        int16_t pid = queue->fifo[queue->head];
        queue->head = (queue->head+1) % QUEUE_MAX_SIZE;
        queue->size -= 1;
        return pid;
    }else{
        return ERROR_QUEUE_EMPTY;
    }
}
