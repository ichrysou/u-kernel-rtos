#include "kernel.h"
#include "heap.h"
#include "stats.h"
#include "hooks.h"
#include "hooks.h"


uint_32 criticalNesting = 0; 
uint_32 interruptNesting = 0;
uint_8 kernel_running = 0;

/* early initialization, must be the first thing to do in main*/
void kernel_init(void)
{
	heap_init();
	task_initTasks();
	port_hwInit();
#if STATS_ENABLED == 1
	stats_init();
#endif
}
/*this is  always last call in main function. It should
 * not be expected to ever return.*/

void kernel_start(void){
     uint_32 i;
     
     /* Find first task to start*/
     task_findHighestPriorityTask();
     currentTCB = highestTCB;
     
     port_tickConfig();
     port_tickStart();
     kernel_running = 1;
     highestTCB->state = RUNNING;

     /* provoke trap  */
     port_startFirstTask();
}

void kernel_schedule(void)
{
	ENTER_CRITICAL();
	/* if we are in interrupt context, perform no Context Switch.
	 * interruptExit() will handle it */
	if(!interruptNesting){
		task_findHighestPriorityTask();
		if (currentTCB == highestTCB){
			EXIT_CRITICAL();
			return;
		}else{
		        highestTCB->state = RUNNING;
			EXIT_CRITICAL();
			
#if CONTEXT_SWITCH_HOOK_ENABLED
			hooks_contextSwitchHook();
#endif
			
			SWITCH_CONTEXT();
		}
	}else{
		EXIT_CRITICAL();
		return;
	}
}


void kernel_yield()
{
	ENTER_CRITICAL();
	currentTCB->state = BLOCKED;

	ReadyArray[currentTCB->prio] = NULL;
#if HIGHEST_PRIO_ALT == 2
	ReadyTaskBitmap[(currentTCB->prio >> 5)] &=  ~(1 << (currentTCB->prio & 0x1F)) ;
	ReadyTaskIndex &= ~(1 << (currentTCB->prio >> 5));
#elif HIGHEST_PRIO_ALT == 3
	ReadyTaskBitmap &=  ~(1 << (currentTCB->prio & 0x1F));
#endif
	
	EXIT_CRITICAL();
	kernel_schedule();
}
