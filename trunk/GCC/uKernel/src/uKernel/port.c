#include "port.h"
#include "lpc17xx_timer.h"

/**
 * @brief: Initialize the stack of a newly created task. 
 * 
 */
OSStackType *StkInit(OSStackType *tos, task function, void *args, OSStackType stack_size)
{
	/* Simulate the stack frame as it was one of an interrupted task */
	#if STACK_GROWTH == 1
	
	OSStackType *stk;
	/*guarantee stack alignment is 4 byte alignment*/
	if (((uint_32)tos & (STACK_ALIGN - 1)) != 0){
		tos = ((uint_32)tos + (uint_32)(STACK_ALIGN - 1)) & ~(STACK_ALIGN - 1);
	}
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
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk
		| SysTick_CTRL_ENABLE_Msk;         /* Enable SysTick IRQ and SysTick Timer */
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


void cpsie(void)
{
	__asm volatile("cpsie i	\n");
}

void cpsid(void)
{
	__asm volatile("cpsid i	\n");
}


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

#if STATS_ENABLED
void port_reset_stat_timer_val(){
	TIM_ResetCounter(LPC_TIM0);
}

void port_stat_timer_init(){


		TIM_TIMERCFG_Type TMR0_Cfg;
		TIM_MATCHCFG_Type TMR0_Match;

		/* On reset, Timer0/1 are enabled (PCTIM0/1 = 1), and Timer2/3 are disabled (PCTIM2/3 = 0).*/
		/* Initialize timer 0, prescale count time of 100uS */
		TMR0_Cfg.PrescaleOption = TIM_PRESCALE_USVAL;
		TMR0_Cfg.PrescaleValue = 1;
		/* Use channel 0, MR0 */
		TMR0_Match.MatchChannel = 0;
		/* Enable interrupt when MR0 matches the value in TC register */
		TMR0_Match.IntOnMatch = ENABLE;
		/* Enable reset on MR0: TIMER will reset if MR0 matches it */
		TMR0_Match.ResetOnMatch = TRUE;
		/* Don't stop on MR0 if MR0 matches it*/
		TMR0_Match.StopOnMatch = FALSE;
		/* Do nothing for external output pin if match (see cmsis help, there are another options) */
		TMR0_Match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
		/* Set Match value, count value of 10000 (10000 * 100uS = 1000000us = 1s --> 1 Hz) */
		TMR0_Match.MatchValue = 10000000;
		/* Set configuration for Tim_config and Tim_MatchConfig */
		TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TMR0_Cfg);
		TIM_ConfigMatch(LPC_TIM0, &TMR0_Match);
		/* Start timer 0 */
		TIM_Cmd(LPC_TIM0, ENABLE);

}

uint_32 port_get_stat_timer_val(void){
	return LPC_TIM0->TC;
}

#endif
