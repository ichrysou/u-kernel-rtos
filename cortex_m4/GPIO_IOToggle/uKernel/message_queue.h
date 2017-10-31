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

queue *queue_create(unsigned int max_size, unsigned int elementSize);
void queue_delete(queue *q);
err_t queue_sendToTail(queue *q, void *msg);

err_t queue_receive(queue *q, void *buff, int timeout);
err_t queue_peek(queue *q, void *buff);


#endif
