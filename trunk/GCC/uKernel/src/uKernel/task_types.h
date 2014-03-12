#ifndef __TASK_TYPES_H__
#define __TASK_TYPES_H__
#include "list.h"
typedef struct task_struct {
	OSStackType *TopOfStkPtr;  /* Top of task's stack*/
	uint_16 prio;      	/* Priority of corresponding task */
	state_t state; 		/* State of task */
	event_state_t estate; //estate reports the event that woke up the task. TODO: Not very nice indeed. find sth else to know when event timed out or actually received
	volatile uint_32 delay;
	struct list_head event_list;
	struct list_head delay_list;
#if MULTIPLE_TASKS_PER_PRIORITY
	struct list_head same_prio_tasks;
	uint_32 time_slice;
#endif
} TCB;
#endif
