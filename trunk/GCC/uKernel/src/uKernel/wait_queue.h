#ifndef __WAIT_QUEUE_H__
#define __WAIT_QUEUE_H__
#include "task.h"

err_t addTaskToWaitQueue(struct list_head *list, TCB *task);
TCB *removeHeadFromWaitQueue(struct list_head *list);
TCB *removeTaskFromWaitQueue(struct list_head *list, uint_32 priority);

#endif
