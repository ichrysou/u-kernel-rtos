#include "stats.h"
#include "kernel.h"
#include "port.h"
volatile uint_32 statCounter = 0;
volatile uint_8 stat_flag = 0;
uint_32 idleCounterStats = 0;

/* systick interrupts should be already configured before this call*/
/* this call only works in StartOS for some strange reason.*/
void statsInit()
{


    ENABLE_INTERRUPTS();

    OSTickConfig();
    OSTickStart();
    while(!stat_flag);
	while(stat_flag){
		 statCounter++;
	}

	DISABLE_INTERRUPTS();
	OSTickStop();
}

uint_32 getCpuUtilization(){

	/* TODO: can be a better way? */
	if (!STATS_ENABLED)
		return 0xFF;
	if (!kernel_running)
		return (uint_32) 0xFFFFFFFF;

	return (uint_32)(100  *  ((statCounter - idleCounterStats) / (float)statCounter));

}
/*needs to be called from the sys tick irq
 */
void stats_hook(){

}
