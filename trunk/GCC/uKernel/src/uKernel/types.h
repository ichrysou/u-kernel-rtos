#ifndef __TYPES_H__
#define __TYPES_H__
#include "port_types.h"

typedef enum {
	ERR_NO_MEM,
	ERR_INVALID_PRIO,
	ERR_UNKNOWN,
	ERR_OK,
	ERR_Q_EMPTY,
	ERR_Q_FULL,
	ERR_Q_TIMEOUT
}err_t;

typedef enum {
	RUNNING,
	READY,
	BLOCKED,		
	SUSPENDED,
}state_t;

typedef enum {
	MESSAGE_QUEUE,
	SEMAPHORE,
	TIMED_OUT,
	NONE
}event_state_t;

typedef void (*task)(void *);

typedef uint_32 OSStackType;


#endif 
