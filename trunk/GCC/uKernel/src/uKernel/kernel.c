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

void idleTaskInit()
{
#if STACK_GROWTH == 1
	task_create(IDLE_TASK_PRIO, IdleTask, NULL, IDLE_STACK_SIZE, &IdleStack[IDLE_STACK_SIZE - 1], NULL);
#else 
	task_create(IDLE_TASK_PRIO, IdleTask, NULL, IDLE_STACK_SIZE, &ildeStack[0], NULL);
#endif
}
