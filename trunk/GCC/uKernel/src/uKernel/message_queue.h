#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__
#include "wait_queue.h"
#include "time.h"

typedef struct queue{
        uint_8 *head;
        uint_8 *tail;
        uint_8 *start;
        uint_8 *end;
        uint_32 max_length;
        uint_32 length;
        uint_32 elementSize;
        struct list_head tasksPost;
        struct list_head tasksPend;
} queue;

queue *queueCreate(unsigned int max_size, unsigned int elementSize);
void queueDelete(queue *q);
err_t queueSendToTail(queue *q, void *msg);
//error_t queueSendToHead(queue *q, void *msg, int timeout);
err_t queueReceive(queue *q, void *buff, int timeout);
err_t queuePeek(queue *q, void *buff);


#endif
