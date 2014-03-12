#ifndef __TASK_H__
#define __TASK_H__
#include "types.h"
#include "kern_conf.h"
#include "list.h"
#include <stdlib.h>

/* ========================================*/
/* ---------------- Macros ------------- */
/* ========================================*/
#define IDLE_PRIO (MAX_TASKS - 1)

/* ========================================*/
/* ---------------- Globals ------------- */
/* ========================================*/
#include "task_types.h"


extern TCB *TaskArray[MAX_TASKS];
extern TCB *ReadyArray[MAX_TASKS];

#if HIGHEST_PRIO_ALT == 2
extern uint_32 ReadyTaskBitmap[(MAX_TASKS >> 5) + 1];
extern uint_32 ReadyTaskIndex;
#elif HIGHEST_PRIO_ALT == 3
extern uint_32 ReadyTaskBitmap;
#endif

extern struct list_head allTasksLinked;

extern TCB *currentTCB;
extern TCB *highestTCB;

/* ======================================*/
/* -------------- Functions -------------*/
/* ======================================*/
/* public interface */
err_t task_create(uint_8 prio, task function, void *args, OSStackType stack_size, OSStackType *tos, TCB *handler);

#if !MULTIPLE_TASKS_PER_PRIORITY
err_t prioEnable(uint_16 prio);
err_t prioDisable(uint_16 prio);
#endif

err_t taskEnable(TCB *tsk);
err_t taskDisable(TCB *tsk);

/* RTOS-private functions */
TCB * TCBAlloc(void);
void TCBInit(TCB *newTCB, uint_8 prio, uint_32 stk_size);
void tasksInit(void);
TCB *getTCBbyPrio(unsigned int prio);
#if HIGHEST_PRIO_ALT != 3
void FindHighestPriorityTask(void);
#endif

void schedule(void);
void yield(void);

#endif
