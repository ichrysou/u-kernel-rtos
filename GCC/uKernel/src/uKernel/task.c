#include "task.h"
#include "kernel.h"
#include "heap.h"
/* ==========================================================*/
/* ------------- Private Function Declarations -------------*/
/* ========================================================*/

static void task_lTCBInit(TCB *newTCB, uint_8 prio, uint_32 stk_size);
/* ========================================*/
/* ---------------- Globals ------------- */
/* ========================================*/

//TODO: Resolve: Memory overhead because of using
//		2 different structures to keep the same thing

/* Global array to hold each and every created task */
TCB *TaskArray[MAX_TASKS];
/* Linked list to hold each and every created task */
struct list_head allTasksLinked;

/* Declarations depend on the way we get the highest priority task.*/
TCB *ReadyArray[MAX_TASKS];
//TODO: PRIO_ALT 2 does not work.. Please fix it
#if HIGHEST_PRIO_ALT == 2
uint_32 ReadyTaskBitmap[(MAX_TASKS >> 5) + 1];
uint_32 ReadyTaskIndex;
#elif HIGHEST_PRIO_ALT == HP_PORT_OPTIMAL
uint_32 ReadyTaskBitmap;
#endif

uint_32 number_of_tasks = 0;


TCB *currentTCB;
TCB *highestTCB;

/* ======================================*/
/* -------------- Functions -------------*/
/* ======================================*/

/*to be called at kernel initialization phase, before any task creation*/
void tasksInit(void)
{
	int i;

	for (i = 0; i < MAX_TASKS; i++)
		TaskArray[i] = NULL;


	for (i = 0; i < MAX_TASKS; i++)
		ReadyArray[i] = NULL;

#if HIGHEST_PRIO_ALT == 2

	for (i = 0; i < (MAX_TASKS >> 5); i++)
		ReadyTaskBitmap[i] = 0;
	ReadyTaskIndex = 0;

#elif HIGHEST_PRIO_ALT == 3
	ReadyTaskBitmap = 0;
#endif
	INIT_LIST_HEAD(&allTasksLinked);
	idleTaskInit();
}
// it seems more convenient to to it like this:
/* uint_8 ReadyTaskBitmap[ (MAX_TASKS) >> 3 + 1];*/
/* Then also uint_8 ReadyTaskIndex.
   Then use a look up table of 256 for finding
   the first bit set. This is architecture independedn real time solution*/
/* to be called after kernel initialization*/

err_t task_create(uint_8 prio, task function, void *args, uint_32 stk_size, uint_32 *tos, TCB *handler)
{
	err_t ret = ERR_OK;
	TCB *newTCB;
	/* Check if task priority is bigger than the maximum allowed number of tasks
	   or if task priority is reserved by another task */

	/*in case of 1-1 prios-tasks only this check would do*/
	if ((prio > MAX_TASKS) || (TaskArray[prio] != NULL)){
			ret = ERR_INVALID_PRIO;
			return ret;
		}


	/* keeping some stats */
	number_of_tasks++;	//number of created tasks
	/* there's always room for moar... */

	/* Initialize TCB and Stack */
 	newTCB = (TCB *)portMalloc(sizeof(TCB));//TCBAlloc();
	task_lTCBInit(newTCB, prio, stk_size);

	/* StkInit is in port */
	newTCB->TopOfStkPtr = StkInit(tos, function, args, stk_size);

	TaskArray[newTCB->prio] = newTCB;

	ReadyArray[newTCB->prio] = newTCB;

#if HIGHEST_PRIO_ALT == 2

	ReadyTaskBitmap[(newTCB->prio >> 5)] |=  1 << (newTCB->prio & 0x1F);
	ReadyTaskIndex |= 1 << (newTCB->prio >> 5);
#elif HIGHEST_PRIO_ALT == 3
	ReadyTaskBitmap |= 1 << (newTCB->prio &0x1F);
#endif
	/*add to list of all linked tasks*/
	list_add_tail(&(newTCB->delay_list) , &allTasksLinked);

	handler = newTCB;
	return ret;
}


static void task_lTCBInit(TCB *newTCB, uint_8 prio, uint_32 stk_size)
{
	newTCB->prio = prio;
	newTCB->state = READY;
	newTCB->estate = NONE;
	newTCB->delay = 0;
	INIT_LIST_HEAD(&(newTCB->delay_list));
	INIT_LIST_HEAD(&(newTCB->event_list));

}


TCB *getTCBbyPrio(unsigned int prio)
{
	if (prio < MAX_TASKS)
		return TaskArray[prio];
	else 
		return NULL;
}

#if HIGHEST_PRIO_ALT == 1

void FindHighestPriorityTask()
{
	uint_32 highestPrio = MAX_PRIO- 1;
	while(ReadyArray[highestPrio] == NULL){
		highestPrio--;
	}
	highestTCB = ReadyArray[highestPrio];
}
//check lg_debruijn
#elif HIGHEST_PRIO_ALT == 2

/* or something like*/
uint_8 clz(uint_32 x)
{
	uint_8 n = 0;
    if (x == 0)
		return 32;
    if ((x & 0xFFFF0000) == 0){
		n = n + 16;
		x = x << 16;
	}
    if ((x & 0xFF000000) == 0){
		n = n + 8;
		x = x << 8;
	}
    if ((x & 0xF0000000) == 0){
		n = n + 4;
		x = x << 4;
	}
    if ((x & 0xC0000000) == 0){
		n = n + 2;
		x = x << 2;
	}
    if ((x & 0x80000000) == 0){
		n = n + 1;
	}		
    return n;
}

void FindHighestPriorityTask()
{
	highestTCB = ReadyArray[31 - clz(ReadyTaskBitmap[31 - clz(ReadyTaskIndex)])];
}
/* Better alternative? */
#endif


err_t prioEnable(uint_16 prio)
{
	
	ENTER_CRITICAL();
	if (TaskArray[prio] == NULL){
		EXIT_CRITICAL();
		return 	ERR_INVALID_PRIO;
	}
	TaskArray[prio]->state = READY;
	
	
	ReadyArray[prio] = TaskArray[prio];
#if HIGHEST_PRIO_ALT == 2
	ReadyTaskBitmap[(prio >> 5)] |= 1 << (prio & 0x1F);
	ReadyTaskIndex |= 1 << (prio >> 5);
	
#elif HIGHEST_PRIO_ALT == 3
	ReadyTaskBitmap |= 1 << (prio & 0x1F);
#endif
	EXIT_CRITICAL();
	schedule();
	return ERR_OK;
}

err_t taskEnable(TCB *tsk)
{
	uint_16 prio = tsk->prio;
	ENTER_CRITICAL();
	if (TaskArray[prio] == NULL){
		EXIT_CRITICAL();
		return 	ERR_INVALID_PRIO;
	}
	tsk->state = READY;


	ReadyArray[prio] = tsk;
#if HIGHEST_PRIO_ALT == 2
	ReadyTaskBitmap[(prio >> 5)] |= 1 << (prio & 0x1F);
	ReadyTaskIndex |= 1 << (prio >> 5);
#elif HIGHEST_PRIO_ALT == 3
	ReadyTaskBitmap |= 1 << (prio & 0x1F);
#endif


	EXIT_CRITICAL();
	/* if taskEnable is not called from within an ISR then call schedule*/
	if(interruptNesting == 0){
		schedule();
	}

	return ERR_OK;
}

err_t prioDisable(uint_16 prio)
{

  
	if (TaskArray[prio] == NULL)
		return ERR_INVALID_PRIO;
	if (prio == currentTCB->prio){
		yield();
		return ERR_OK;
	}else{
		ENTER_CRITICAL();
		TaskArray[prio]->state = BLOCKED;
		ReadyArray[prio] = NULL;
#if HIGHSET_PRIO_ALT == 2
		ReadyTaskBitmap[ (prio >>5)] &= ~(1 << (prio & 0x1F));
		ReadyTaskIndex &= ~(1 << (prio >> 5));
#elif HIGHEST_PRIO_ALT == 3
		ReadyTaskBitmap &= ~(1 << (prio & 0x1F));
#endif
		EXIT_CRITICAL();
		return ERR_OK;
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


