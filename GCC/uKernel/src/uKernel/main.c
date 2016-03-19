#include "task.h"
#include "LPC17xx.h"
#include "dotmatgl.h"
#include "kernel.h"
#include "heap.h"
#include "message_queue.h"
#include "sem.h"
/* #include "lpc17xx_adc.h" */
/* #include "lpc17xx_pwm.h" */
/* #include "lpc17xx_pinsel.h" */
/* #include "lpc_types.h" */

#if HOOKS_ENABLED
#include "hooks.h"
#endif

void Task1(void *args);
void Task2(void *args);
void Task3(void *args);
void Task4(void *args);
void Task5(void *args);
void Task6(void *args);
void TaskMatrix(void *args);
void ADC_Task(void *args);
const int j = 1;

#define is_little_endian() ((*(char *) &j) == 1)
/* prios increase from 0 to 32*/
#define TASK1_PRIO 2
#define TASK2_PRIO 3
#define TASK3_PRIO 4
#define TASK4_PRIO 5
#define TASK5_PRIO 6 //MOD: prio changed to 6
#define TASK6_PRIO 7
#define TASKMAT_PRIO 8
#define ADC_TASK_PRIO 9

OSStackType Stack1[STACK_SIZE];
OSStackType Stack2[STACK_SIZE];
OSStackType Stack3[STACK_SIZE];
OSStackType Stack4[STACK_SIZE];
OSStackType Stack5[STACK_SIZE];
OSStackType Stack6[STACK_SIZE];
OSStackType StackMatrix[STACK_SIZE];
OSStackType StackADC[STACK_SIZE];

queue *task6Q;
queue *taskMatrixQueue;

sem *s;
sem *task1_factorial;



/* PWM_TIMERCFG_Type PWMCfgDat; */
/* PWM_MATCHCFG_Type PWMMatchCfgDat; */
/* PINSEL_CFG_Type PinCfg; */


int main()
{

  LPC_GPIO1->FIODIR |= 1 << 18;
  LPC_GPIO1->FIODIR |= 1 << 20;
  LPC_GPIO1->FIODIR |= 1 << 21;
  LPC_GPIO1->FIODIR |= 1 << 23;

  uKern_Init();
  /* don't create any tasks before uKern_Init()!!! */
  /* create ur tasks */
  task_create(TASK1_PRIO, Task1, NULL, STACK_SIZE, &Stack1[STACK_SIZE - 1], NULL);
  task_create(TASK2_PRIO, Task2, NULL, STACK_SIZE, &Stack2[STACK_SIZE - 1], NULL);
  task_create(TASK3_PRIO, Task3, NULL, STACK_SIZE, &Stack3[STACK_SIZE - 1], NULL);
  task_create(TASK4_PRIO, Task4, NULL, STACK_SIZE, &Stack4[STACK_SIZE - 1], NULL);
  task_create(TASK5_PRIO, Task5, NULL, STACK_SIZE, &Stack5[STACK_SIZE - 1], NULL);
  task_create(TASK6_PRIO, Task6, NULL, STACK_SIZE, &Stack6[STACK_SIZE - 1], NULL);
  /* task_create(TASKMAT_PRIO, TaskMatrix, NULL, STACK_SIZE, &StackMatrix[STACK_SIZE - 1], NULL); */
  /* task_create(ADC_TASK_PRIO, ADC_Task, NULL, STACK_SIZE, &StackADC[STACK_SIZE - 1], NULL); */

  /*init other services*/

  task6Q = queueCreate(10, sizeof(uint_32));

  taskMatrixQueue = queueCreate(10, sizeof(uint_32));
  s = sem_init(1);
  task1_factorial = sem_init(1);
  StartOS();
  /* We actually do not expect to ever reach here */
  return;
}

//factorial of 100
uint_32 fact_result;
void Task1(void *args)
{
  args = (void *)0;
  args = (void *)((uint_32 )args + 1);
  int i = 0;
  uint_32 fact = 1;


  while(1){
    fact = 1;

    for (i = 0; i < 50; i++){
      ;;
    }
    for (i = 1; i < 10; i++){
      fact = fact*(i + 1);
    }
    sem_get(task1_factorial,100);
    fact_result = fact;
    sem_release(task1_factorial);
    LPC_GPIO1->FIOPIN ^= 1 << 18;
    timeDelay(100);
  }
}
void ADC_IRQHandler(void)
{
  uint_32 adcValue;

  interruptEnter();

  /* if (ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_0, ADC_DATA_DONE)) */
  /* { */
  /* //adcValue = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0); */
  /* adcValue = ADC_GlobalGetData(LPC_ADC); */
  /* } */

  interruptExit();
}



void ADC_Task(void *args)
{
  /*ADC init*/
  //ADC_Init(LPC_ADC, 2000);
  //ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
  //
  //ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, ENABLE);
  //ADC_BurstCmd(LPC_ADC, ENABLE);
  //NVIC_EnableIRQ(ADC_IRQn);
  while(1){

    LPC_GPIO1->FIOPIN ^= 1 << 20;

    timeDelay(100);

  }

}
void TaskMatrix(void *args)
{
  unsigned char message[] = {0, 0, 0, 0, 0, 0x1F, 0x8, 0x4, 0x8, 0x1F, 0, 0x10, 0x8,\
			     0x7, 0x8, 0x10, 0x0, 0x0, 0x0, 0x1F, 0x1, 0x1 , 0x0, 0x1F, 0x11, 0x1F, 0x0, 0x18, 0x6, 0x1, 0x6, 0x18, 0x0, 0x1F, 0x15, 0x15};


  int meslen = sizeof(message);
  unsigned short int phase = 0;
  //dotmat_init();
  int cntr = 0;
  int fast = 0;
  uint_8 temp;

  while(1){

    /* unplot_col(fast); */
    fast = (++fast) % MAT_MAX_COL;
    /* plot_col(fast); */
    temp =  message[(fast + phase) % meslen] ;
    /* plot_val(temp); */
    if((cntr++ % 250) == 0)
      phase = (phase + 1) % meslen;
    timeDelay(100);

  }

}
//prime numbas (stop after 10 first)
uint_32 prime_res;
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
	primes[cntr++ % 100] = i;  // this array overlaps with our stack after index 0x51  or sth.. that is very weird.
	found = 0;

      }
    }
    prime_res = primes[0];
    timeDelay( 200);//yield();//prioEnable(TASK1_PRIO);
    LPC_GPIO1->FIOPIN ^= 1 << 20;

    //PWM_MatchUpdate(LPC_PWM1, 6, 100 - i++, PWM_MATCH_UPDATE_NOW);
    /* timeDelay( 100); */
  }
}

/* fibonacci task */
void Task3(void *args)
{
  
  args = (void *)0;
  args = (void *)((uint_32 )args + 1);
  uint_32 fibonacci[100];
  int i;
  while(1){
    fibonacci[0] = 0;
    fibonacci[1] = 1;
    for (i = 2; i < 100; i++)
      fibonacci[i] = fibonacci[i - 1] + fibonacci[(i - 2)];

    queueSendToTail(task6Q, (void *)&fibonacci[3]);
    /* sem_release(s); */
    LPC_GPIO1->FIOPIN ^= 1 << 23;
    timeDelay(100);

  }
}
err_t semaphore;
void Task4(void *args){
  

  while(1){
    
    semaphore = sem_get(s, 500);
    
    LPC_GPIO1->FIOPIN ^= 1 << 21;
    /*timeDelay(1000);*/
  }
}

void Task5(void *args){
  args = (void *)0;
  args = (void *)((uint_32 )args + 1);
  uint_32 i = 0;
  while(1){
    sem_get(s, 0);
    timeDelay(300);

    yield();
    for (i = 0; i < 100; i++){
#if STATS_ENABLED
      getCpuUtilization();
#endif
    }
  }
}

void Task6(void *args){
  args = (void *)0;
  args = (void *)((uint_32 )args + 1);
  uint_32 *buff = portMalloc(sizeof(uint_32));
  /*addded sth here*/
  while(1){

    queueReceive(task6Q, buff, 0);

    timeDelay(500);
  }
}


