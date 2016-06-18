#include "message_queue.h"
#include "kernel.h"
#include "heap.h"

static void queueGetMessage(void *buff, queue *q);

queue *queueCreate(unsigned int max_length, unsigned int elementSize)
{
     queue *q =(queue *) heapMalloc(sizeof(queue));
     if (q == NULL){
	  return NULL;
     }
     q->start =(uint_8 *) heapMalloc(elementSize * max_length);
     if (q->start == NULL){
	  return NULL;
     }
     q->end = q->start + elementSize * max_length;
     q->head = q->start;
     q->tail = q->start;
     q->max_length = max_length;
     q->length = 0;
     q->elementSize = elementSize;
     INIT_LIST_HEAD(&(q->tasksPost));
     INIT_LIST_HEAD(&(q->tasksPend));
     return q;
}

void queueDelete(queue *q)
{
     heapFree(q);
}

err_t queueSendToTail(queue *q, void *msg)
{
     TCB *tmp;
     ENTER_CRITICAL();
     if (q->length == q->max_length){
	  EXIT_CRITICAL();
	  return ERR_Q_FULL;
     }

     if (q->tail + q->elementSize >= q->end){
	  q->tail = q->start;
     }
     portMemcpy((void *)q->tail, msg, q->elementSize);
     q->tail = q->tail + q->elementSize;
     q->length++;
     if (!list_empty(&(q->tasksPend))){
	  tmp = removeHeadFromWaitQueue(&(q->tasksPend));
	  if (tmp->delay){
	       tmp->delay = 0;
	       /* remove also from delayed list */
	  }

	  taskEnable(tmp);
     }
     EXIT_CRITICAL();
     return ERR_OK;
}

err_t queueReceive(queue *q, void *buff, int timeout)
{
     ENTER_CRITICAL();
     /* is there a message in the queue?*/
     if (q->length){
	  /* if yes just get it and return */
	  currentTCB->estate = NONE;
	  queueGetMessage(buff, q);
	  EXIT_CRITICAL();
	  return ERR_OK;
     }else{
	  /* if not then add self in waitqueue*/
	  addTaskToWaitQueue(&(q->tasksPend), currentTCB);
	  /* update task state*/
	  currentTCB->estate = MESSAGE_QUEUE;
	  /* timeout pend requested?*/
	  if (timeout){
	       /* if yes, add also delay*/
	       timeDelay(timeout);
	       EXIT_CRITICAL();
	       /*switch will hit here?*/
	       if (currentTCB->estate == TIMED_OUT){
		    /*remove self from waitqueue*/
		    removeTaskFromWaitQueue(&(q->tasksPend), currentTCB->prio);
		    return ERR_Q_TIMEOUT;
	       }
	  }else{
	       EXIT_CRITICAL();
	       /* no timeout then just yield*/
	       yield();
	       /* returned task, just received a message */
	  }
	  ENTER_CRITICAL();
	  currentTCB->estate = NONE;
	  queueGetMessage(buff, q);
	  EXIT_CRITICAL();
	  return ERR_OK;
     }
}

err_t queuePeek(queue *q, void *buff)
{
     err_t ret;
     ENTER_CRITICAL();
     /* is there a message in the queue?*/
     if (q->length){
	  /* if yes just get it and return */
	  currentTCB->estate = NONE;
	  queueGetMessage(buff, q);
	  ret = ERR_OK;
     }else{
	  /*Queue is empty just leave*/
	  ret = ERR_Q_EMPTY;
     }
     EXIT_CRITICAL();
     return ret;
}

uint_32 queueGetNumberOfMessages(queue *q)
{
     return q->length;
}

static void queueGetMessage(void *buff, queue *q)
{
     portMemcpy(buff, q->head, q->elementSize);
     q->length--;
     if (q->head + q->elementSize >= q->end){
	  q->head = q->start;
     }else{
	  q->head += q->elementSize;
     }
}

