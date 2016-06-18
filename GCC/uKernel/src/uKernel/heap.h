#ifndef __HEAP_H__
#define __HEAP_H__
#include "kern_conf.h"
#include "types.h"

#define HEAP_SIZE CONF_HEAP_SIZE
#define HEAP_ALIGN CONF_HEAP_ALIGN

void heapInit();
void *heapMalloc(uint_32 sizeRequested);
uint_32 heapGetHeapSize();
void heapFree(void *ptr);

#endif
