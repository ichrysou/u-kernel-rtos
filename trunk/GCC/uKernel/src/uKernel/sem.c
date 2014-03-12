#include "sem.h"
#include "kernel.h"
#include "heap.h"
#include "wait_queue.h"
#include "time.h"

/* give one for binary semaphore */
sem *sem_init(uint_32 count)
{
	sem *tmp = (sem *)portMalloc(sizeof(sem));
	tmp->cnt = count;
	INIT_LIST_HEAD(&(tmp->task_queue));
	return tmp;
}

void sem_free(sem *sem)
{
	portFree(sem);
}
/* call with timeout as 0 if you want to wait forever*/
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
	if	(timeout){
		timeDelay(timeout);
		EXIT_CRITICAL();
		/*context switch will hit here?*/
		/* exit point 2*/
		if (currentTCB->estate == TIMED_OUT){
			/* remove self form waitqueue*/
			//TODO: use TCB* instead of prio for finding which task to remove
			//		from waitqueue. Serves us better for MULTILPE.....
			removeTaskFromWaitQueue(&(s->task_queue), currentTCB->prio);

			/*abort attempt*/
			return ERR_Q_TIMEOUT;
		}
	}else{

		EXIT_CRITICAL();
		yield();
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
//TODO:	//this is for binary semaphores
//		if (s->cnt == 1){
//			EXIT_CRITICAL();
//			return;
//		}
		(s->cnt)++;

	}else{
		tmp = removeHeadFromWaitQueue(&(s->task_queue));
		if (tmp->delay){
			tmp->delay = 0;
		}
		//TODO: use code instead of function TaskEnable here
		//   	this is because when MULTIPLE_TASKS_PER_PRIORITY
		//		is enabled, tmp->prio disables the whole priority
		//      rather than the single task
		prioEnable(tmp->prio);
	}
	EXIT_CRITICAL();
	return;

}
