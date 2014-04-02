#include "kernel.h"
#include "heap.h"


//#include "LPC17xx.h"

OSStackType IdleStack[IDLE_STACK_SIZE];

uint_32 criticalNesting = 0; 
uint_32 interruptNesting = 0;
uint_32 idleCounter = 0;

void uKern_Init(void)
{

	heapInit();
	tasksInit();


	OSTickConfig();
}

void StartOS(void){

	FindHighestPriorityTask();
	currentTCB = highestTCB;
	OSTickStart();
	init_service();
	//start_os();
	
}

void schedule(void)
{
	ENTER_CRITICAL();
	/* if we are in interrupt context, perform no Context Switch.
	 * interruptExit() will handle it */
	if(!interruptNesting){
		FindHighestPriorityTask();
		if (currentTCB == highestTCB){
			EXIT_CRITICAL();
			return;
		}else{
			EXIT_CRITICAL();
			SWITCH_CONTEXT();
		}
	}else{
		EXIT_CRITICAL();
		return;
	}
}
void IdleTask(void *args)
{
	while(1){

		idleCounter++;

#if IDLE_TASK_SLEEP
		LPC_SC->PCON = 0x00;
		__WFI();
#endif
#if HOOKS_ENABLED && IDLE_TASK_HOOK 
		idleTaskHook();
#endif
	}
}



void yield()
{
	ENTER_CRITICAL();
	currentTCB->state = BLOCKED;
//TODO: you need to think this through.
//  	We need to delete the tsk from somewhere,
//		even for PRIO_ALT 2, 3.
#if MULTIPLE_TASKS_PER_PRIORITY
	if (list_empty(&(currentTCB->same_prio_tasks))){
#endif
	ReadyArray[currentTCB->prio] = NULL;
#if HIGHEST_PRIO_ALT == 2
	ReadyTaskBitmap[(currentTCB->prio >> 5)] &=  ~(1 << (currentTCB->prio & 0x1F)) ;
	ReadyTaskIndex &= ~(1 << (currentTCB->prio >> 5));
#elif HIGHEST_PRIO_ALT == 3
	ReadyTaskBitmap &=  ~(1 << (currentTCB->prio & 0x1F));
#endif

#if MULTIPLE_TASKS_PER_PRIORITY
	}else{
		//TODO: check if this is correct, I mean if the correct TCB is removed from the list
		ReadyArray[currentTCB->prio] = list_entry(currentTCB->same_prio_tasks.next, TCB, same_prio_tasks);
		//TODO: this can be for all three alternatives
		list_del(&currentTCB->same_prio_tasks);
	}
#endif

	EXIT_CRITICAL();
	schedule();
}
