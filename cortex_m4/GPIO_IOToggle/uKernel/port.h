#ifndef __PORT_H__
#define __PORT_H__
//#include "LPC17xx.h"
#include "../Drivers/CMSIS/Device/ST/STM32F4xx/Include/stm32f401xe.h"
#include "kern_conf.h"
#include "types.h"

#define configKERNEL_INTERRUPT_PRIORITY 255
#define portNVIC_PENDSV_PRI (((unsigned long)configKERNEL_INTERRUPT_PRIORITY) << 16)
#define portNVIC_SYSTICK_PRI (((unsigned long)configKERNEL_INTERRUPT_PRIORITY) << 24)
#define portNVIC_SYSPRI2 ((volatile unsigned long *)0xe000ed20)

void port_enableInterrupts(void);
void port_disableInterrupts(void);

#define DISABLE_INTERRUPTS() port_disableInterrupts()
#define ENABLE_INTERRUPTS() port_enableInterrupts()

#define STACK_GROWTH 1  // 1 if stack grows from High to Low memory

#define SWITCH_CONTEXT() \
    { SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; }

/**TODO: 2) Create the alternative implementation for STACK_GROWTH 0
 * @brief: Port specific function. Used to initialize the stack of a newly
 *         created task. Is must look as if it was just interrupted.
 */

OSStackType *port_stackInit(OSStackType *tos, task function, void *args, OSStackType stack_size);

uint_32 port_tickConfig(void);
void port_tickStart(void);
void port_tickStop(void);
void port_sleep(void);
void port_startFirstTask();
void HardFaultHndlr(void);
void *port_memcpy(void *dest, const void *src, uint_32 n);

#if HIGHEST_PRIO_ALT == 3
#if MAX_PRIO > 32
#error \
    "Number of task priorities is too damn high for using port specific FindHighestPriorityTask.\
		Either use less tasks or change HIGHEST_PRIO_ALT in kern_conf.h"
#endif

static unsigned char __clz(ulBitmap) {
    unsigned char ucReturn;
    __asm volatile("clz %0, %1" : "=r"(ucReturn) : "r"(ulBitmap));
    return ucReturn;
}

#define task_findHighestPriorityTask() \
    { highestTCB = ReadyArray[31 - __clz(ReadyTaskBitmap)]; }
#endif

void port_hwInit(void);

#ifdef DEBUG
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress);
#endif

#endif
