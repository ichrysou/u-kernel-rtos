#include "time.h"
#include "kernel.h"
#include "task.h"

static uint_32 new_var = 0;
static volatile uint_32 OSTicks = 0;
extern uint_8 stat_counter_not_ready;
extern uint_8 stat_calibration;
static float_32 cpuUtilization = 0;
float_32 utilizations[100];
uint_32  Ticks[100];
extern uint_32 idleCounter_Max;
uint_32 i = 0;
void OSTick(void){
	
	TCB *tmp;
	struct list_head *iter1;
	
	interruptEnter();

	/* increase tick counter*/
	OSTicks++;
	/* check task list for delayed tasks*/
	ENTER_CRITICAL();
	list_for_each(iter1, &allTasksLinked){
		tmp = list_entry(iter1, TCB, delay_list);
		/* is anyone delayed?*/
		if (tmp->delay != 0){
			/* tick 'em */
			tmp->delay--;
			/* if someone is expired */
			if(tmp->delay == 0){
				/* event state is checked to see if  */
				/* there are any tasks waiting for an event */
				/* with timeout */
				if(tmp->estate != NONE){
					/* if so, change estate */
					tmp->estate = TIMED_OUT;
				}
				taskEnable(tmp);
			}
		}
	}
	
	EXIT_CRITICAL();
	interruptExit();
}


void timeDelay(uint_32 ticks){
	
	ENTER_CRITICAL();
	
	currentTCB->delay = ticks;
	currentTCB->state = BLOCKED;
	
	ReadyArray[currentTCB->prio] = NULL;
#if HIGHEST_PRIO_ALT == 2
	ReadyTaskBitmap[(currentTCB->prio >> 5)] &=  ~(1 << (currentTCB->prio & 0x1F)) ;	
	ReadyTaskIndex &= ~(1 >> currentTCB->prio >> 5);
#elif HIGHEST_PRIO_ALT == 3
	ReadyTaskBitmap &=  ~(1 << (currentTCB->prio & 0x1F)) ;
	
	EXIT_CRITICAL();
	schedule();
#endif
}

uint_32 getTickCount(void)
{
	return OSTicks;
}
