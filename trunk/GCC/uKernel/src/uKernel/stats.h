#ifndef __STATS_H__
#define __STATS_H__
#include "port_types.h"
extern volatile uint_8 stat_flag;
extern uint_32 idleCounterStats;

void statsInit();
uint_32 getCpuUtilization();
void stats_hook();


#endif
