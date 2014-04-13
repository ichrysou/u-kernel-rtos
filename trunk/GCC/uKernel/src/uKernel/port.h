#ifndef __PORT_H__
#define __PORT_H__
#include "kern_conf.h"
#include "types.h"
#include "LPC17xx.h"



#define configKERNEL_INTERRUPT_PRIORITY 255
#define portNVIC_PENDSV_PRI                     ( ( ( unsigned long ) configKERNEL_INTERRUPT_PRIORITY ) << 16 )
#define portNVIC_SYSTICK_PRI            ( ( ( unsigned long ) configKERNEL_INTERRUPT_PRIORITY ) << 24 )
#define portNVIC_SYSPRI2                        ( ( volatile unsigned long *) 0xe000ed20 )
/* critical region handling */
//naive implementation
//__inline and asm should go to compiler abstraction layer
void cpsie();
void cpsid(void);

#define  DISABLE_INTERRUPTS() cpsid()
#define  ENABLE_INTERRUPTS()  cpsie()

#define STACK_GROWTH 1  // 1 if stack grows from High to Low memory

#define SWITCH_CONTEXT() {SCB->ICSR |= 1 << 28;}

/**TODO: 2) Create the alternative implementation for STACK_GROWTH 0
 * @brief: Port specific function. Used to initialize the stack of a newly
 *         created task. Is must look as if it was just interrupted.
 */

OSStackType *StkInit(OSStackType *tos, task function, void *args, OSStackType stack_size);

void swap(void);

void init_service(void);
uint_32 OSTickConfig(void);
void OSTickStart(void);
void OSStartFirstTask();
void HardFaultHndlr(void);
#if HIGHEST_PRIO_ALT == 3
#if MAX_PRIO > 32
#error "Number of task priorities is too damn high for using port specific FindHighestPriorityTask.\
		Either use less tasks or change HIGHEST_PRIO_ALT in kern_conf.h"
#endif

__attribute__( ( always_inline ) ) static unsigned char __clz( ulBitmap )
{
	unsigned char ucReturn;
	__asm volatile ( "clz %0, %1" : "=r" ( ucReturn ) : "r" ( ulBitmap ) );
	return ucReturn;
}

#define FindHighestPriorityTask()	{highestTCB = ReadyArray[31 - __clz(ReadyTaskBitmap)];}
#endif

// system calls declarations here
//compiler abstraction?

void context_switch(void) __attribute__(( naked ));
void start_first_task(void);
void hw_init(void);


#ifdef DEBUG
void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress );
#endif


#endif





/*
void start_os(void);
 */
