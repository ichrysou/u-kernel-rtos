#include "stats.h"
#include "kernel.h"
#include "port.h"
#include "time.h"
#include "task.h"

#if STATS_ENABLED

OSStackType StatTaskStack[STAT_TASK_STACK_SIZE];


static uint_32 idleTimerStamp = 0;
static uint_32 idleTimerTime = 0;
static uint_32 statTimerTime = 0;
static uint_32 statTimerStamp = 0;
static float_32 cpuUtilization = 0;
float_32 utilizations[100];

void StatTask(void *args)
{
	uint_32 i = 0;
	uint_32 temp;
	while(1){
		idleTimerTime = 0;
		port_reset_stat_timer_val();
		/*care must be taken that timer overflow time is less than total delay of statistic task,so
		 * that the cpu utilization calculation is valid*/
		statTimerStamp = port_get_stat_timer_val();
		timeDelay(100);
		temp = port_get_stat_timer_val();
		statTimerTime = temp - statTimerStamp;/*TIM_GetCounter(TIM2);*/
		cpuUtilization = (float_32) (100 * ((statTimerTime- idleTimerTime) / (float)statTimerTime));
		utilizations[i++ % 100] = cpuUtilization;
		
	}
}
/* systick interrupts should be already configured before this call*/
/* this call only works in StartOS for some strange reason.*/
void statsInit()
{

	port_stat_timer_init();
	task_create(STAT_TASK_PRIO,
		    StatTask,
		    NULL,
		    STAT_TASK_STACK_SIZE,
		    &StatTaskStack[STAT_TASK_STACK_SIZE - 1],
		    0);
	
}

float_32 getCpuUtilization()
{
	return (float_32)cpuUtilization;
}

/*needs to be called from the context switch hook
 */
 /** Bug: You need to call this somehow also from the interruptEnter/Exit routine, so that 
 *        the interrupt service time is not calculated in the idleTimerTime variable.
 *        In general 3 states we have:
 *        1) Idle Task -> Interrupt Enter
 *        2) InterruptExit -> Idle Task
 *        3) InterruptExit -> Idle Task with Contex Switch.
 *        Desired behavior:
 *        1) Stamp the timer that was already counting since entry to the idle task from cases 2) and 3). Add the difference with stamp from 2 and 3 to idleTimerTime.
 *        2) Stamp the timer in idleTimerStamp
 *        3) Stamp the timer in idleTimerStamp.
 */

void stats_hook(){
	uint_32 temp;
	/* two cases:
	   interrupt nesting > 0
	   interrupt nesting == 0*/
	temp = port_get_stat_timer_val();
	if (interruptNesting == 1){
		if (highestTCB == TaskArray[IDLE_TASK_PRIO]){ /* first case: interrupt occured on idle task context */
			idleTimerTime += temp - idleTimerStamp;
			
		}
		
	}else if (interruptNesting == 0){
		if (highestTCB == TaskArray[IDLE_TASK_PRIO]){
			idleTimerStamp = temp;
			
		}
		
	}
}
#endif
