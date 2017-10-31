#include "message_queue.h"
#include "kernel.h"
#include "heap.h"

static void queue_getMessage(void *buff, queue *q);

queue *queue_create(unsigned int max_length, unsigned int elementSize)
{
     queue *q =(queue *) heap_malloc(sizeof(queue));
     if (q == NULL){
	  return NULL;
     }
     q->start =(uint_8 *) heap_malloc(elementSize * max_length);
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

void queue_delete(queue *q)
{
     heap_free(q);
}

err_t queue_sendToTail(queue *q, void *msg)
{
     TCB *tmp;
     ENTER_CRITICAL();
     if (q->length == q->max_length){
	  EXIT_CRITICAL();
	  return ERR_Q_FULL;
     }

     port_memcpy((void *)q->tail, msg, q->elementSize);
     q->tail = q->tail + q->elementSize;
     if (q->tail >= q->end){
	  q->tail = q->start;
     }
     
     q->length++;
     if (!list_empty(&(q->tasksPend))){
	  tmp = wait_queue_removeHead(&(q->tasksPend));
	  if (tmp->delay){
	       tmp->delay = 0;
	       /* remove also from delayed list */
	  }

	  task_taskEnable(tmp);
     }
     EXIT_CRITICAL();
     return ERR_OK;
}

err_t queue_receive(queue *q, void *buff, int timeout)
{
     ENTER_CRITICAL();
     /* is there a message in the queue?*/
     if (q->length){
	  /* if yes just get it and return */
	  currentTCB->estate = NONE;
	  queue_getMessage(buff, q);
	  EXIT_CRITICAL();
	  return ERR_OK;
     }else{
	  /* if not then add self in waitqueue*/
	  wait_queue_addTask(&(q->tasksPend), currentTCB);
	  /* update task state*/
	  currentTCB->estate = MESSAGE_QUEUE;
	  /* timeout pend requested?*/
	  if (timeout){
	       /* if yes, add also delay*/
	       time_delay(timeout);
	       EXIT_CRITICAL();
	       /*switch will hit here?*/
	       if (currentTCB->estate == TIMED_OUT){
		    /*remove self from waitqueue*/
		    wait_queue_removeTask(&(q->tasksPend), currentTCB->prio);
		    return ERR_Q_TIMEOUT;
	       }
	  }else{
	       EXIT_CRITICAL();
	       /* no timeout then just kernel_yield*/
	       kernel_yield();
	       /* returned task, just received a message */
	  }
	  ENTER_CRITICAL();
	  currentTCB->estate = NONE;
	  queue_getMessage(buff, q);
	  EXIT_CRITICAL();
	  return ERR_OK;
     }
}

err_t queue_peek(queue *q, void *buff)
{
     err_t ret;
     ENTER_CRITICAL();
     /* is there a message in the queue?*/
     if (q->length){
	  /* if yes just get it and return */
	  currentTCB->estate = NONE;
	  queue_getMessage(buff, q);
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

static void queue_getMessage(void *buff, queue *q)
{
     port_memcpy(buff, q->head, q->elementSize);
     q->length--;
     if (q->head + q->elementSize >= q->end){
	  q->head = q->start;
     }else{
	  q->head += q->elementSize;
     }
}

