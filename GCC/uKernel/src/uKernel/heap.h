#ifndef __HEAP_H__
#define __HEAP_H__
#include "kern_conf.h"
#include "types.h"

#define HEAP_SIZE CONF_HEAP_SIZE
#define HEAP_ALIGN CONF_HEAP_ALIGN

void heap_init();
void *heap_malloc(uint_32 sizeRequested);
uint_32 heap_getHeapSize();
void heap_free(void *ptr);

#endif
