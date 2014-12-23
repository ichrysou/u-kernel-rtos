#include "kernel.h"
#include "heap.h"

#include "hooks.h"
OSStackType IdleStack[IDLE_STACK_SIZE];

uint_32 criticalNesting = 0; 
uint_32 interruptNesting = 0;
volatile uint_32 idleCounter = 0;
uint_8 kernel_running = 0;

/* early initialization, must be the first thing to do in main*/
void uKern_Init(void)
{

	heapInit();
	tasksInit();
	hw_init();
	
#if STATS_ENABLED
	statsInit();
#endif
	
}
/*this is  always last call in main function. It should
 * not be expected to ever return.*/
void StartOS(void){
	/* Find first task to start*/
	FindHighestPriorityTask();
	currentTCB = highestTCB;
	/*enable interrupts*/
	OSTickConfig();
	OSTickStart();
	kernel_running = 1;
	/* provoke trap  */
	OSStartFirstTask();
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
			
#if CONTEXT_SWITCH_HOOK_ENABLED
			contextSwitchHook();
#endif
#if STATS_ENABLED
			stats_hook();
#endif
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

#if HOOKS_ENABLED && IDLE_TASK_HOOK_ENABLED
		idleTaskHook();
#endif
	}
}



void yield()
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
	schedule();
}
