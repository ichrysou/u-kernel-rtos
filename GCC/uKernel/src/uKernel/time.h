#ifndef __TIME_H__
#define __TIME_H__
#include "types.h"

void OSTick(void);
void TickTasks(void);
void timeDelay(uint_32 ticks);
uint_32 getTickCount(void);

#endif
