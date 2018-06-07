#include "sem.h"
#include "heap.h"
#include "kernel.h"
#include "time.h"
#include "wait_queue.h"

/* give one for binary semaphore */
sem *sem_init(uint_32 count) {
    sem *tmp = (sem *)heap_malloc(sizeof(sem));
    tmp->cnt = count;
    INIT_LIST_HEAD(&(tmp->task_queue));
    return tmp;
}

void sem_free(sem *sem) { heap_free(sem); }

/* if timeout is 0 wait forever */
err_t sem_get(sem *s, uint_32 timeout) {
    ENTER_CRITICAL();
    if (s->cnt > 0) {
        s->cnt--;
        EXIT_CRITICAL();
        /* exit point 1*/
        return ERR_OK;
    }
    // TODO: do it NOW, currentTCB->estate = SEMAPHORE
    currentTCB->estate = SEMAPHORE;
    wait_queue_addTask(&(s->task_queue), currentTCB);
    if (timeout) {
        time_delay(timeout);
        EXIT_CRITICAL();
        /*context switch will hit here?*/
        /* exit point 2*/
        if (currentTCB->estate == TIMED_OUT) {
            wait_queue_removeTask(&(s->task_queue), currentTCB->prio);

            return ERR_Q_TIMEOUT;
        }
    } else {
        EXIT_CRITICAL();
        kernel_yield();
        /*context switch will hit here?*/
        /* exit point 3*/
    }
    currentTCB->estate = NONE;
    return ERR_OK;
}

void sem_release(sem *s) {
    TCB *tmp;

    ENTER_CRITICAL();
    if (list_empty(&(s->task_queue))) {
        (s->cnt)++;
    } else {
        tmp = wait_queue_removeHead(&(s->task_queue));
        if (tmp->delay) {
            tmp->delay = 0;
        }
        task_prioEnable(tmp->prio);
    }
    EXIT_CRITICAL();
    return;
}
