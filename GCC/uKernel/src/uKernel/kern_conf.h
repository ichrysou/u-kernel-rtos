#ifndef __KERN_CONF__
#define __KERN_CONF__

#define MAX_TASKS 32

#define IDLE_TASK_SLEEP 0

#define MAX_PRIO MAX_TASKS
#define CONF_HEAP_SIZE 1000
#define CONF_HEAP_ALIGN 4
#define STACK_ALIGN 4
#define STACK_SIZE 350 //in words 
#define IDLE_STACK_SIZE 150
#define OS_TICK_FREQ 1000 // in ticks/sec
#define CPU_FREQ 100000000uL
//#define __DEBUG__
#define HP_NAIVE 1
#define HP_PORT_OPTIMAL 3
#define HIGHEST_PRIO_ALT HP_PORT_OPTIMAL

#define IDLE_TASK_HOOK_ENABLED 1
#define HOOKS_ENABLED 0
#define STATS_ENABLED 1
#define CONTEXT_SWITCH_HOOK_ENABLED 1
#define TIME_SLICE 10 //in ticks

#if HIGHEST_PRIO_ALT == HP_PORT_OPTIMAL && MAX_TASKS > 32
#error "When using port accelerated context switch, you can't have more than 32 tasks"
#endif

#define MEM_MANG 0
#define WAIT_QUEUE_IMPL 1
#endif
