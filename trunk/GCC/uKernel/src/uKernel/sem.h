#ifndef __SEM_H__
#define __SEM_H__

#include "types.h"
#include "list.h"

typedef struct semaphore {
	uint_32 cnt;
	struct list_head task_queue;
}sem;

sem *sem_init(uint_32 count);
void sem_free(sem *sem);

/*void P()*/
err_t sem_get(sem *s, uint_32 timeout);

	

/*void V()*/
void sem_release(sem *s);

#endif
