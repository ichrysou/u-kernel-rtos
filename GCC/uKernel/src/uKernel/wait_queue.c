#include "wait_queue.h"
#include "kernel.h"


err_t addTaskToWaitQueue(struct list_head *list, TCB *task)
{
	
	struct list_head *tmp;
	ENTER_CRITICAL();
	if (list_empty(list)){
		list_add(&(task->event_list), list);
	}else{
		/* for(tmp = list; (list_entry(tmp->prev, TCB, event_list)->prio) <= (task->prio); tmp = tmp->prev){ */
		/* 	;; */
		/* } */
		list_add_tail(&(task->event_list), list);
	}
	EXIT_CRITICAL();	
	
	return ERR_OK;
}

TCB *removeTaskFromWaitQueue(struct list_head *list, uint_32 priority)
{
	TCB *tmp;
	struct list_head *iter;
	
	ENTER_CRITICAL();
	if (list_empty(list)){
		EXIT_CRITICAL();
			return NULL;
	}
	//BUG: list_for_each_safe should be used instead

	list_for_each(iter, list){
		tmp = list_entry(iter, TCB, event_list);
		if (tmp->prio == priority){
			list_del_init(iter);
			//break works as well
			break;
		}
	}
	
	EXIT_CRITICAL();
	return tmp;
};


TCB *removeHeadFromWaitQueue(struct list_head *list)
{
	TCB *tmp;
	
	ENTER_CRITICAL();
	if (list_empty(list)){
		EXIT_CRITICAL();
		return NULL;
	}
	tmp = list_entry(list->next, TCB, event_list);
	list_del(list->next);
	EXIT_CRITICAL();
	return tmp;
	
}






