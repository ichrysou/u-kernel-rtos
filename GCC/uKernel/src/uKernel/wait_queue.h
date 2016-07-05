#ifndef __WAIT_QUEUE_H__
#define __WAIT_QUEUE_H__
#include "task.h"

err_t wait_queue_addTask(struct list_head *list, TCB *task);
TCB *wait_queue_removeHead(struct list_head *list);
TCB *wiat_queue_removeTask(struct list_head *list, uint_32 priority);

#endif
