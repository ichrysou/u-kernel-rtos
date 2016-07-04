#include "sem.h"
#include "kernel.h"
#include "heap.h"
#include "wait_queue.h"
#include "time.h"

/* give one for binary semaphore */
sem *sem_init(uint_32 count)
{
     sem *tmp = (sem *)heapMalloc(sizeof(sem));
     tmp->cnt = count;
     INIT_LIST_HEAD(&(tmp->task_queue));
     return tmp;
}

void sem_free(sem *sem)
{
     heapFree(sem);
}


err_t sem_get(sem *s, uint_32 timeout)
{

     ENTER_CRITICAL();
     if (s->cnt > 0){
	  s->cnt--;
	  EXIT_CRITICAL();
	  /* exit point 1*/
	  return ERR_OK;
     }
     //TODO: do it NOW, currentTCB->estate = SEMAPHORE
     currentTCB->estate = SEMAPHORE;
     addTaskToWaitQueue( &(s->task_queue) , currentTCB);
     if(timeout){
	  timeDelay(timeout);
	  EXIT_CRITICAL();
	  /*context switch will hit here?*/
	  /* exit point 2*/
	  if (currentTCB->estate == TIMED_OUT){
	       removeTaskFromWaitQueue(&(s->task_queue), currentTCB->prio);

	       return ERR_Q_TIMEOUT;
	  }
     }else{

	  EXIT_CRITICAL();
	  kernel_yield();
	  /*context switch will hit here?*/
	  /* exit point 3*/
     }
     currentTCB->estate = NONE;
     return ERR_OK;
	
}

void sem_release(sem *s)
{
     TCB *tmp;
	
     ENTER_CRITICAL();
     if (list_empty(&(s->task_queue)))
     {
	  (s->cnt)++;
     }else{
	  tmp = removeHeadFromWaitQueue(&(s->task_queue));
	  if (tmp->delay){
	       tmp->delay = 0;
	  }
	  prioEnable(tmp->prio);
     }
     EXIT_CRITICAL();
     return;
}
