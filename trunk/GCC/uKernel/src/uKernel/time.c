#include "time.h"
#include "kernel.h"
#include "task.h"
#if STATS_ENABLED
#include "stats.h"
#endif
//Time.c
/* Sys Tick counter */
static volatile unsigned int OSTicks = 0;


void OSTick(void){


	TCB *tmp;
	struct list_head *iter1, *iter2;
/* TODO:here we should have OS_TICK_HOOK in general.
 *  in that hook an if the stats were enabled then
 *  we would have the stats_hook*/
#if STATS_ENABLED
	/* this is for initializing the stats counter*/
	if (!kernel_running){
		stat_flag ^= 1;
		return;
	}
#endif

    interruptEnter();

#if STATS_ENABLED
    /*idle counter snapshot and reset*/
    idleCounterStats = idleCounter;
    idleCounter = 0;
#endif
	/* increase tick counter*/
	//LPC_GPIO1->FIOPIN |= 1 << 18;
	//LPC_GPIO1->FIOPIN &= ~(1 << 20);
	OSTicks++;
	/* check task list for delayed tasks*/
	ENTER_CRITICAL();
	list_for_each(iter1, &allTasksLinked){
		tmp = list_entry(iter1, TCB, delay_list);
		/* is anyone delayed?*/
		if (tmp->delay != 0){
			/* tick 'im */
			tmp->delay--;
			/* if someone is expired */
			if(tmp->delay == 0){
				tmp->state = READY;
				/* event state is checked to see if 
				there are any tasks waiting for an event
				with timeout */
				if(tmp->estate != NONE){
					/* if so, change estate */
					tmp->estate = TIMED_OUT;
				}

#if MULTIPLE_TASKS_PER_PRIORITY
				if(ReadyArray[tmp->prio] != NULL)
					list_add_tail(&tmp->same_prio_tasks, ReadyArray[tmp->prio]->same_prio_tasks);
				else
#endif
				ReadyArray[tmp->prio] = tmp;
#if HIGHEST_PRIO_ALT == 2
				ReadyTaskBitmap[(tmp->prio >> 5)] |= 1 << (tmp->prio & 0x1F);								
				ReadyTaskIndex |= 1 << (tmp->prio >> 5);
#elif HIGHEST_PRIO_ALT == 3
				ReadyTaskBitmap |= 1 << (tmp->prio & 0x1F);
#endif

			}
		}
	}
//#if MULTIPLE_TASKS_PER_PRIORITY
//		if (currentTCB->time_slice){
//				currentTCB->time_slice--;
//				if (!currentTCB->time_slice){
//					ReadyArray[currentTCB->prio] = list_entry((&(currentTCB->same_prio_tasks))->next, TCB, same_prio_tasks);
//					currentTCB->state = BLOCKED;
//					currentTCB->time_slice = TIME_SLICE;
//				}
//		}
//#endif
	EXIT_CRITICAL();
	interruptExit();
}
/* but the way, to ipmlement this you'll need:
   a) TCB linked list with all the created task's TCBs
   b) Interrupt enter and interrupt exit functions to be called on every interrupt
	  entry and exit
*/
void TickTasks(void){
	;
}

/*almost just as yield()*/
//TODO: add #if MULTIPLE_TASKS_PER_PRIORITY  ...
void timeDelay(uint_32 ticks){
	
	ENTER_CRITICAL();

	currentTCB->delay = ticks;
	currentTCB->state = BLOCKED;


#define TEST_IT_WORKS
#ifdef TEST_IT_WORKS
	EXIT_CRITICAL();
	yield();
#else
	ReadyArray[currentTCB->prio] = NULL;
#if HIGHEST_PRIO_ALT == 2
	ReadyTaskBitmap[(currentTCB->prio >> 5)] &=  ~(1 << (currentTCB->prio & 0x1F)) ;	
	ReadyTaskIndex &= ~(1 >> currentTCB->prio >> 5);
#elif HIGHEST_PRIO_ALT == 3
	ReadyTaskBitmap &=  ~(1 << (currentTCB->prio & 0x1F)) ;
#endif
	EXIT_CRITICAL();
	schedule();
#endif
}

uint_32 getTickCount(void)
{
	return OSTicks;
}
