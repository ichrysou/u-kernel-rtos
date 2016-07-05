#include "port.h"
#include "lpc17xx_timer.h"

void port_sleep(void)
{
     LPC_SC->PCON = 0x00;
     __WFI();
}
void *port_memcpy(void *dest, const void *src, uint_32 n)
{
     uint_32 i;
     uint_8 *loc_dest, *loc_src;
     void *loc_retDest;

     loc_retDest = dest;
     loc_dest = (uint_8 *)dest;
     loc_src = (uint_8 *)src;
     
     for (i = 0; i < n; i++){
	  loc_dest[i] = loc_src[i];
     }
     return loc_retDest;
}


/**
 * @brief: Initialize the stack of a newly created task. 
 * 
 */
OSStackType *port_stackInit(OSStackType *tos, task function, void *args, OSStackType stack_size)
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


/* Set the msp back to the start of the stack. */
void port_startFirstTask(){

     __asm volatile(
	  /* " ldr r0, =0xE000ED08   \n"      /\* Use *\/ */
	  /* " ldr r0, [r0]          \n" */
	  /* " ldr r0, [r0]          \n" */
	  /* " msr msp, r0           \n"       */
	  " cpsie i               \n"      /* Globally enable interrupts. */
	  " svc 0                 \n"      /* System call to start first task. */
	  " nop                   \n"
	  );
	
}

void port_startFirstTaskHandler(void)
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

void port_tickStart(void){
     SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk
	  | SysTick_CTRL_ENABLE_Msk;         /* Enable SysTick IRQ and SysTick Timer */
}

void port_tickStop(void){
     SysTick->CTRL  &= ~(SysTick_CTRL_ENABLE_Msk);                    /* Disable SysTick IRQ and SysTick Timer */
}

uint_32 port_tickConfig(void){
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
void HardFault_Handler(void) __attribute__ ((naked));

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


void port_enableInterrupts(void)
{
     __asm volatile("cpsie i	\n");
}

void port_disableInterrupts(void)
{
     __asm volatile("cpsid i	\n");
}


void port_contextSwitch(void)
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



void port_hwInit(){
#ifdef __DEBUG__
     SCB->VTOR = 0x10000000;
#endif
     /* Make PendSV, CallSV and SysTick the same priroity as the kernel. */
     *(portNVIC_SYSPRI2) |= portNVIC_PENDSV_PRI;
     *(portNVIC_SYSPRI2) |= portNVIC_SYSTICK_PRI;

}


void HardFault_Handler(void)
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

