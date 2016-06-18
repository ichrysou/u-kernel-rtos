#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"

#include "stats.h"
#include "task.h"
#include "kernel.h"
#include "heap.h"
#include "message_queue.h"
#include "sem.h"

/* prios increase from 0 to 32*/
#define TASK1_PRIO 2
#define TASK2_PRIO 3
#define TASK3_PRIO 4
#define TASK4_PRIO 5
#define TASK5_PRIO 6
#define TASK6_PRIO 7
#define TASK7_PRIO 8


queue *task6Q;
sem *s;
sem *task1_factorial;
uint_32 fact_result;
LPC_UART_TypeDef *UARTx;
uint8_t rxBuf[16] = {0};
uint32_t rxCnt  = 0;
volatile Bool rxFlag = FALSE;

OSStackType Stack1[STACK_SIZE];
OSStackType Stack2[STACK_SIZE];
OSStackType Stack3[STACK_SIZE];
OSStackType Stack4[STACK_SIZE];
OSStackType Stack5[STACK_SIZE];
OSStackType Stack6[STACK_SIZE];
OSStackType Stack7[STACK_SIZE];


void PIN_Init(void);
void UART3_Init(void);
void init_uart(void);
void UART3_IRQHandler(void);

void Task1(void *args);
void Task2(void *args);
void Task3(void *args);
void Task4(void *args);
void Task5(void *args);
void Task6(void *args);
void Task7(void *args);




int main()
{

     /* initalize rtOS */
     uKern_Init();

     /* don't create any tasks before uKern_Init()!!! */
     task_create(TASK1_PRIO, Task1, NULL, STACK_SIZE, &Stack1[STACK_SIZE - 1], NULL);
     task_create(TASK2_PRIO, Task2, NULL, STACK_SIZE, &Stack2[STACK_SIZE - 1], NULL);
     task_create(TASK3_PRIO, Task3, NULL, STACK_SIZE, &Stack3[STACK_SIZE - 1], NULL);
     task_create(TASK4_PRIO, Task4, NULL, STACK_SIZE, &Stack4[STACK_SIZE - 1], NULL);
     task_create(TASK5_PRIO, Task5, NULL, STACK_SIZE, &Stack5[STACK_SIZE - 1], NULL);
     task_create(TASK6_PRIO, Task6, NULL, STACK_SIZE, &Stack6[STACK_SIZE - 1], NULL);
     task_create(TASK7_PRIO, Task7, NULL, STACK_SIZE, &Stack7[STACK_SIZE - 1], NULL);

     /*init other OS services*/
     task6Q = queueCreate(10, 16 * sizeof(uint_8));
     s = sem_init(1);
     task1_factorial = sem_init(1);

     /* Start the kernel */
     StartOS();
     
     /* We actually do not expect to ever reach here */
     return 0;
}

/*factorial of 100*/
void Task1(void *args)
{
     args = (void *)0;
     args = (void *)((uint_32 )args + 1);
     int i = 0;
     uint_32 fact = 1;

  
     while(1){
	  fact = 1;

	  for (i = 1; i < 10; i++){
	       fact = fact*(i + 1);
	  }
	  sem_get(task1_factorial, 0);
	  fact_result = fact;
	  sem_release(task1_factorial);
	  
	  timeDelay(100);
     }
}


/* prime numbas (stop after 10 first) */

void Task2(void *args)
{
     args = (void *)0;
     args = (void *)((uint_32 )args + 1);
     int i = 0,j;
     uint_32 primes[100];
     uint_8 found = 0;
     int cntr = 0;
     while(1){
    
	  for (i = 0; i < 100 ; i++){

	       for (j = 2; j < i; j++){
		    found = 1;
		    if (((i % j) == 0)) {
			 found = 0;
			 break;
		    }
	       }
	       if (found){
		    primes[cntr++ % 100] = i;
		    found = 0;

	       }
	  }
	  primes[0];
	  sem_get(task1_factorial, 0);
	  fact_result = 0xAA;
	  sem_release(task1_factorial);
    
	  timeDelay( 200);
     }
}

/* fibonacci task */
void Task3(void *args)
{
  
     args = (void *)0;
     args = (void *)((uint_32 )args + 1);
     uint_32 fibonacci[100];
     uint_32 cntr = 0;
     int i;
     while(1){
	  fibonacci[0] = 0;
	  fibonacci[1] = 1;
	  for (i = 2; i < 1000; i++)
	       fibonacci[i%100] = fibonacci[(i - 1)%100] + fibonacci[(i - 2)%100];
	  
	  timeDelay(200);
     }
}

void Task4(void *args){
  
     err_t semaphore;
     while(1){
    
	  semaphore = sem_get(s, 100);
    
	  LPC_GPIO1->FIOPIN ^= 1 << 21;
	  /*timeDelay(1000);*/
     }
}

void Task5(void *args){
     args = (void *)0;
     uint_32 i = 0;
     float_32 cpuLoad;
     while(1){

	  timeDelay(30);

	  for (i = 0; i < 100; i++){

	       cpuLoad = getCpuUtilization();

	  }
     }
}

void Task6(void *args){
     args = (void *)0;
     err_t queue_err;
     uint_8 *buff = heapMalloc(16 * sizeof(uint_8));
     uint_32 loc_rxCnt;
     /*addded sth here*/
     while(1){
	  
	  /* } */
	  
	  queueReceive(task6Q,(void *) buff, 0);
	  LPC_GPIO1->FIOPIN ^= 1 << 20;

	  /* /\* limit of TX FIFO is 16 bytes *\/ */
	  UART_Send(UARTx, (uint_8 *)&buff[1], ((uint_32)buff[0]) & 0xFF, NONE_BLOCKING);
	  timeDelay(10);

       
     }
}


void Task7(void *args)
{


     init_uart();

     while(1){
	  /* if (rxFlag == TRUE) { */
	  /*      rxFlag = FALSE; */
	  /*      UART_Send(LPC_UART3, rxBuf, rxCnt, BLOCKING); */
	  /* } else { */
	  if (rxFlag == TRUE){
	       UART_Send(LPC_UART3, (uint8_t *)"hello word\n", sizeof("hello word\n"), NONE_BLOCKING);
	  }
	  /* } */
 	  timeDelay(100);
	  LPC_GPIO1->FIOPIN ^= 1 << 23;	       
     }
}

void init_uart(void)
{
  /** System init */
     LPC_GPIO1->FIODIR |= 1 << 18;
     LPC_GPIO1->FIODIR |= 1 << 20;
     LPC_GPIO1->FIODIR |= 1 << 21;
     LPC_GPIO1->FIODIR |= 1 << 23;

  unsigned int tmp, i;


  /** UART init */
  PIN_Init();
  UART3_Init();

}
void PIN_Init(void)
{
  /* Pin configuration for UART3 */
  PINSEL_CFG_Type PinCfg;

  /*
   * Initialize UART3 pin connect
   */
  LPC_PINCON->PINSEL0 |= 0b1010;
  /* PinCfg.Funcnum   = 2; */
  /* PinCfg.OpenDrain = 0; */
  /* PinCfg.Pinmode   = 0; */
  /* PinCfg.Portnum   = UART3_PORT; */
  /* PinCfg.Pinnum    = UART3_TX; */
  /* PINSEL_ConfigPin(&PinCfg); */
  /* PinCfg.Pinnum    = UART3_RX; */
  /* PINSEL_ConfigPin(&PinCfg); */
}

void UART3_Init(void)
{
  UARTx = (LPC_UART_TypeDef *)LPC_UART3;

  /* UART Configuration structure variable */
  UART_CFG_Type UARTConfigStruct;
  /* UART FIFO configuration Struct variable */
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;

  /* Initialize UART Configuration parameter structure to default state:
   * Baudrate = 9600bps
   * 8 data bit
   * 1 Stop bit
   * None parity
   */
  UART_ConfigStructInit(&UARTConfigStruct);

  /* Set Baudrate to 115200 */
  UARTConfigStruct.Baud_rate = 9600;

  /* Initialize UART3 peripheral with given to corresponding parameter */
  UART_Init(UARTx, &UARTConfigStruct);

  /* Initialize FIFOConfigStruct to default state:
   * - FIFO_DMAMode = DISABLE
   * - FIFO_Level = UART_FIFO_TRGLEV0
   * - FIFO_ResetRxBuf = ENABLE
   * - FIFO_ResetTxBuf = ENABLE
   * - FIFO_State = ENABLE
   */
  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
  UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV2;/* 8 Bytes */
  /* Initialize FIFO for UART3 peripheral */
  UART_FIFOConfig(UARTx, &UARTFIFOConfigStruct);

  /** config and enable interrupter */
  UART_IntConfig(UARTx, UART_INTCFG_RBR, ENABLE);

  /** NVIC enable interrupter , it's requesté”›ï¿½*/
  NVIC_EnableIRQ(UART3_IRQn);

  /*  Enable UART Transmit */
  UART_TxCmd(UARTx, ENABLE);
}

void UART3_IRQHandler(void)
{
     uint8_t ReceiveValue;
     uint8_t i;
     uint32_t IIRValue;
     uint32_t loc_rxCnt;     
     
     
     loc_rxCnt = 0;
     IIRValue = UART_GetIntId(LPC_UART3);
     
     while ((IIRValue & 0x01) == 0) {
	  switch (IIRValue & 0x0E) {/* flag */
	  case UART_IIR_INTID_RDA :
	       for (i = 0; i < 8; i++) {
		    /* rxFlag = TRUE; */
		    ReceiveValue = UART_ReceiveByte(LPC_UART3);
		    rxBuf[1 + loc_rxCnt++] = ReceiveValue;
	       }
	       ENTER_CRITICAL();
	       rxBuf[0] = loc_rxCnt;
	       queueSendToTail(task6Q, (void *)&rxBuf[0]);
	       EXIT_CRITICAL();
	       LPC_GPIO1->FIOPIN ^= 1 << 18;
	       break;
	  case UART_IIR_INTID_CTI:
	       /* rxFlag = TRUE; */
	       ReceiveValue = UART_ReceiveByte(LPC_UART3);
	       rxBuf[1 + loc_rxCnt++] = ReceiveValue;
	       ENTER_CRITICAL();
	       rxBuf[0] = loc_rxCnt;
	       queueSendToTail(task6Q, (void *)&rxBuf[0]);
	       EXIT_CRITICAL();
	       LPC_GPIO1->FIOPIN ^= 1 << 18;
	       break;
	       
	  default:
	       break;
	  }
	  IIRValue = UART_GetIntId(LPC_UART3);
     }  /* if have another interrupter hand on */
}
