#include "port.h"

/**
 * @brief: Initialize the stack of a newly created task. 
 * 
 */
OSStackType *StkInit(OSStackType *tos, task function, void *args, OSStackType stack_size)
{
	/* Simulate the stack frame as it was one of an interrupted task */
	#if STACK_GROWTH == 1
	
	OSStackType *stk;
  	stk = tos;//this causes #bug 0: bos + stack_size - 1;
	*--stk = 0x01000000; //xPSR Reserved, value 1 is for the t-bit
	
	*--stk = (uint_32)function;  //PC reserved
	*--stk = 0; //  LR reserved
	
	stk -= 4; // R12, R3, R2, R1
	*--stk = (uint_32)args; // r0 -- function arguments
	stk -= 8; // r4 - r11, space is reserved.
	return stk;
	#endif
}

void OSStartFirstTask(){

	__asm volatile(
					 " ldr r0, =0xE000ED08   \n" /* Use the NVIC offset register to locate the stack. */
					 " ldr r0, [r0]                  \n"
					 " ldr r0, [r0]                  \n"
					 " msr msp, r0                   \n" /* Set the msp back to the start of the stack. */
					 " cpsie i                               \n" /* Globally enable interrupts. */
					 " svc 0                                 \n" /* System call to start first task. */
					 " nop                                   \n"
			);
	
}
/* enriched alternative*/
void start_first_task(void)
{
	__asm volatile(
			"ldr r2, highestTCBconst2		\n"
			"ldr r1, [r2]				\n"
			"ldr r0, [r1]				\n"
			"ldmia r0!, {r4-r11}		\n"
			"msr psp, r0				\n"
			//"orr lr, lr, #0x4			\n"
			"orr r14, #0xd			\n"
			"bx r14						\n"
			" .align 2					\n"
			"highestTCBconst2:  .word highestTCB\n"
			);
}

void OSTickStart(void){
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
}

void OSTickStop(void){
	SysTick->CTRL  &= ~(SysTick_CTRL_ENABLE_Msk);                    /* Disable SysTick IRQ and SysTick Timer */
}

uint_32 OSTickConfig(void){
  unsigned int ticks = CPU_FREQ / OS_TICK_FREQ;
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
                                                               
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  return (0);               
}	


#ifdef __DEBUG__
volatile uint32_t r0;
volatile uint32_t r1;
volatile uint32_t r2;
volatile uint32_t r3;
volatile uint32_t r12;
volatile uint32_t lr; /* Link register. */
volatile uint32_t pc; /* Program counter. */
volatile uint32_t psr;/* Program status register. */

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
/* These are volatile to try and prevent the compiler/linker optimising them
away as the variables never actually get used.  If the debugger won't show the
values of the variables, make them global my moving their declaration outside
of this function. */

    r0 = pulFaultStackAddress[ 0 ];
    r1 = pulFaultStackAddress[ 1 ];
    r2 = pulFaultStackAddress[ 2 ];
    r3 = pulFaultStackAddress[ 3 ];

    r12 = pulFaultStackAddress[ 4 ];
    lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    psr = pulFaultStackAddress[ 7 ];


    /* When the following line is hit, the variables contain the register values. */
    for( ;; );
}
#endif

void cpsie(void)
{
	__asm volatile(
			"cpsie i	\n"
		);
};

void cpsid(void)
{
	__asm volatile(
			"cpsid i	\n"
			);
};


void context_switch(void)
{
	__asm volatile(

			"mrs  r0, psp				\n"
			"stmdb r0!, {r4-r11} 		\n"
			"ldr r1, currentTCBconst;	\n"
			"ldr r2, [r1]					\n"
			"str r0, [r2]					\n"
			"ldr r2, highestTCBconst 		\n"
			"ldr r1, [r2]					\n"
			"ldr r0, [r1]					\n"
			"ldmia r0!, {r4-r11}			\n"
			"msr psp, r0					\n"
			"ldr r0, currentTCBconst		\n"
			"ldr r1, highestTCBconst		\n"
			"ldr r2, [r1]				\n"
			"str r2, [r0]				\n"
			"bx r14						\n"
			" 	.align 2\n"
			"currentTCBconst: .word currentTCB\n"
			"highestTCBconst: .word highestTCB\n"
			);
}



void hw_init(){
#ifdef __DEBUG__
	SCB->VTOR = 0x10000000;
#endif
    /* Make PendSV, CallSV and SysTick the same priroity as the kernel. */
    *(portNVIC_SYSPRI2) |= portNVIC_PENDSV_PRI;
    *(portNVIC_SYSPRI2) |= portNVIC_SYSTICK_PRI;

}
void init_service(void)
{

    OSStartFirstTask();
}

void HardFaultHndlr(void)
{
	__asm volatile(
			"tst lr, #4		\n"
			"ite eq			\n"
			"mrseq r0, msp  \n"
			"mrsne r0, psp  \n"
			"ldr r1, [r0, #24]\n"
			"b prvGetRegistersFromStack	\n"
			);
}
/*
void start_os(void){
	__asm volatile(

            " svc 0                                 \n"
            " nop                                   \n"
			);
}

void SVC_Handler(void)
{

	__asm volatile(
			"tst lr, #4		\n"
			//"mrseq r0, msp  \n"
			//"mrsne r0, psp  \n"
			"b MainSVCHandler \n"
			"bx lr			\n "
			);
}
void PendSV_Handler(void)
{
	__asm volatile("b context_switch \n");
}
*/
/*__asm void FindHighestPriorityTask()
{
	PRESERVE 8
	IMPORT ReadyTaskBitmap
	IMPORT highestTCB
	IMPORT ReadyTaskArray
	ldr r0, =ReadyTaskBitmap
	ldr r0, [r0]
	rbit r1, r0
	clz r0, r1
	ldr r1, =ReadyTaskArray
	ldr r2, [r1]
	ldr r0, [r2 + r]
}*/
