#include "kernel.h"
#include "heap.h"
#include "stats.h"
#include "hooks.h"
#include "hooks.h"
OSStackType IdleStack[IDLE_STACK_SIZE];

uint_32 criticalNesting = 0; 
uint_32 interruptNesting = 0;
uint_32 idleCounter = 0;
uint_8 kernel_running = 0;

/* early initialization, must be the first thing to do in main*/
void uKern_Init(void)
{
	heapInit();
	tasksInit();
	hw_init();
	
#if STATS_ENABLED == 1
	statsInit();
#endif
}
/*this is  always last call in main function. It should
 * not be expected to ever return.*/
void StartOS(void){
     uint_32 i;
     
     /* Find first task to start*/
     FindHighestPriorityTask();
     currentTCB = highestTCB;
     
     OSTickConfig();

/* #if STATS_ENABLED == 1 */
     /* stats_calculate_idleMax(); */
/* #else */
     OSTickStart();
/* #endif */
     kernel_running = 1;
     highestTCB->state = RUNNING;
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
		        highestTCB->state = RUNNING;
			EXIT_CRITICAL();
			
#if CONTEXT_SWITCH_HOOK_ENABLED
			contextSwitchHook();
#endif
			
			SWITCH_CONTEXT();
		}
	}else{
		EXIT_CRITICAL();
		return;
	}
}
uint_8 always_true;
void IdleTask(void *args)
{
        always_true = 1;
	while(always_true){
	  #if STATS_ENABLED == 1
	  DISABLE_INTERRUPTS();
	  #endif
		idleCounter++;
	  #if STATS_ENABLED == 1		
	  ENABLE_INTERRUPTS();
	  #endif	  
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
