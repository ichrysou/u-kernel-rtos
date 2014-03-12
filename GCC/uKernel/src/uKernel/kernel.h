#ifndef __KERNEL_C__
#define __KERNEL_C__

#include "port.h"
//#include "task.h"
#include <stdio.h>
#include "task_types.h"

/*********************GLOBALS*******************************/
extern TCB *highestTCB;
extern TCB *currentTCB;

extern TCB *TaskArray[MAX_TASKS];
extern TCB *ReadyArray[MAX_TASKS];

#if HIGHEST_PRIO_ALT == 2
extern uint_32 ReadyTaskBitmap[(MAX_TASKS >> 5) + 1];
extern uint_32 ReadyTaskIndex;
#elif HIGHEST_PRIO_ALT == 3
extern uint_32 ReadyTaskBitmap;
#endif


#if HOOKS_ENABLED
#include "hooks.h"
#endif
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  (5 << 3)

#define IDLE_TASK_PRIO 0


#define ENTER_CRITICAL() enterCritical()
#define EXIT_CRITICAL()  exitCritical()

#define IntEnter() interruptEnter()
#define IntExit() interruptExit()

extern uint_32 criticalNesting;
extern uint_32 interruptNesting;

static __inline void enterCritical()
{
	if (!criticalNesting)
		DISABLE_INTERRUPTS();
	criticalNesting++; 
}

static __inline void exitCritical(){
	if (--criticalNesting == 0)
			ENABLE_INTERRUPTS();
}

static __inline void interruptEnter(void)
{
	ENTER_CRITICAL();
	interruptNesting++;
	EXIT_CRITICAL();
}

static __inline void interruptExit(void)
{
	/*TODO: This is kind of relevant (a lot) to the
	 * architecture. Should be moved to port.c*/
	ENTER_CRITICAL();
	if(--interruptNesting == 0){
		FindHighestPriorityTask();
		if (highestTCB != currentTCB)
			SWITCH_CONTEXT();

	}
	EXIT_CRITICAL();

}
void uKern_Init(void);
void StartOS(void);
void IdleTask(void *args);
void idleTaskInit(void);
#endif
