#ifndef __STATS_H__
#define __STATS_H__
#include "port_types.h"
#include "kern_conf.h"
extern volatile uint_8 stat_flag;


#if HIGHEST_PRIO_ALT == 3
#define STAT_TASK_PRIO 10
#endif

#define STAT_TASK_STACK_SIZE 100

void stats_init(void);
float_32 stats_getCpuUtilization(void);
void stats_hook(void);

#endif
