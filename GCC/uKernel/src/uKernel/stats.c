#include "stats.h"
#include "kernel.h"
#include "port.h"
#include "time.h"
#include "task.h"

#if STATS_ENABLED

uint_8 stat_counter_not_ready;
OSStackType StatTaskStack[STAT_TASK_STACK_SIZE];
uint_32 idleCounter_Max;
static float_32 cpuUtilization = 0;
float_32 utilizations[100];
uint_8 stat_idle_counter_ready;
uint_8 stat_calibration;


void StatTask(void *args)
{
     TCB *stat_task_cb;
     uint_32 i = 0;
     uint_16 prio = 0;
     uint_8 first_time;
     idleCounter_Max = 0;
     first_time = 1;

     while(1) {
	  /* clear the idle counter */
	  idleCounter = 0;
	  
	  /* disable all other tasks */
	  if (first_time){
	    for (prio = 1; prio < MAX_TASKS; prio++){
	      if (prio != currentTCB->prio){
		prioDisable(prio);
	      }
	    }

	  }
	  /* wait for 1000 ticks */
	  timeDelay(1000);

	  /* collect idleCounter max value */
	  if (first_time){
	    idleCounter_Max = idleCounter;
	    first_time = 0;
	    for (prio = 1; prio < MAX_TASKS; prio++){
	      if (prio != currentTCB->prio){
		prioEnable(prio);
	      }
	    }
	  }
	  
	  cpuUtilization = (float_32 ) 100 * (1 - (float_32) idleCounter / idleCounter_Max);
	  utilizations[i++ % 100] = cpuUtilization;
     }
}

/* systick interrupts should be already configured before this call*/
/* this call only works in StartOS for some strange reason.*/
void statsInit()
{
  /* stat_counter_not_ready = 1; */
  /* idleCounter_Max = 0; */
  /* stat_calibration = 0; */
     task_create(STAT_TASK_PRIO,
     		 StatTask,
     		 NULL,
     		 STAT_TASK_STACK_SIZE,
     		 &StatTaskStack[STAT_TASK_STACK_SIZE - 1],
     		 0);
}

void stats_calculate_idleMax(void)
{
    
     OSTickStart();
     ENABLE_INTERRUPTS();
     while(stat_counter_not_ready){
     DISABLE_INTERRUPTS();	/* this is just here to mimic the same code as the idle task */
       idleCounter_Max++;
     ENABLE_INTERRUPTS();       
     }
     stat_calibration = 1;
     DISABLE_INTERRUPTS();
}

float_32 getCpuUtilization()
{
     return (float_32)cpuUtilization;
}

void stats_hook(void)
{
  ;;
}


#endif
