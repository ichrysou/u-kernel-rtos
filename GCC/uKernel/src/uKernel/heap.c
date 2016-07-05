#include "heap.h"
#include "kernel.h"


static int heap_start, heap_end;

uint_8 heap1[HEAP_SIZE + HEAP_ALIGN - 1];
uint_8 *heap;

/* to be called before OSTickStart();*/
void heap_init()
{
	uint_32 i;
       if (HEAP_ALIGN % 2 != 0)
         return;
	/* force heap alignment */
	/* There is a better way using union, like in FreeRTOS, but that would be copying. */
	heap =(uint_8 *) (((uint_32)heap1 + HEAP_ALIGN - 1) & ~(HEAP_ALIGN - 1));
	for (i = 0; i < HEAP_SIZE; i++){
		heap[i] = 0;
	}
}

void *heap_malloc(uint_32 sizeRequested)
{

	void *ret;
	ENTER_CRITICAL();
	if (heap_end + sizeRequested > HEAP_SIZE) {
		//if ((heap_end + sizeRequested*/) % HEAP_SIZE > heap_start)//MOD1:

		if(sizeRequested > heap_start)
			ret = NULL;
		else {
			ret = (void *)&heap[0];
			heap_end += sizeRequested - HEAP_SIZE;
		}

	}
	else{

		ret = (void *)&heap[heap_end];// + sizeRequested];//MOD3:
		heap_end += sizeRequested;

	}
	EXIT_CRITICAL();
	return ret;
}

void heap_free(void *ptr)
{
	return;
}

/**
 * @brief: Returns the size of free heap
 */
uint_32 heap_getHeapSize(){
	return (heap_end > heap_start) ? (heap_end - heap_start) : (heap_end + HEAP_SIZE - heap_start);
}


