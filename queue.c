//ReferĂȘncia: https://gist.github.com/ArnonEilat/4471278
#include "gestor.h"

QUEUE *ConstructQueue(int limit) {
    QUEUE *queue = (QUEUE*) malloc(sizeof (QUEUE));
    if (queue == NULL) {
        return NULL;
    }
    if (limit <= 0) {
        limit = 65535;
    }
    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void DestructQueue(QUEUE *queue) {
    PCB* pN;
    while (!isEmpty(queue)) {
        pN = Dequeue(queue);
        free(pN);
    }
    free(queue);
}

int Enqueue(QUEUE *pQueue, PCB *item) {
    /* Bad parameter */
    if ((pQueue == NULL) || (item == NULL)) {
        return FALSE;
    }
    // if(pQueue->limit != 0)
    if (pQueue->size >= pQueue->limit) {
        return FALSE;
    }
    /*the queue is empty*/
    item->prev = NULL;
    if (pQueue->size == 0) {
        pQueue->head = item;
        pQueue->tail = item;

    } else {
        /*adding item to the end of the queue*/
        pQueue->tail->prev = item;
        pQueue->tail = item;
    }
    pQueue->size++;
    return TRUE;
}

PCB* Dequeue(QUEUE *pQueue) {
    /*the queue is empty or bad param*/
    PCB *item;
    if (isEmpty(pQueue))
        return NULL;
    item = pQueue->head;
    pQueue->head = (pQueue->head)->prev;
    pQueue->size--;
    return item;
}

int isEmpty(QUEUE* pQueue) {
    /*if (pQueue == NULL) {
        return FALSE;
    }*/
    if (pQueue->size == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

//-----Imprimir uma lista para testar
void displayQueue(QUEUE* Prontos)
{
    int count=0;
    while(count < Prontos->size){
        PCB* aux=Dequeue(Prontos);
        printf("%s--", aux->nome);
        Enqueue(Prontos, aux);
        count++;
    }
}