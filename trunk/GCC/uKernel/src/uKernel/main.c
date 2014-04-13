#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "lpc17xx.h"
#include "dotmatgl.h"
#include "kernel.h"
#include "heap.h"
#include "message_queue.h"
#include "sem.h"
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
const int j = 1;
#define is_little_endian() ((*(char *) &j) == 1)
/* prios increase from 0 to 32*/
#define TASK1_PRIO 1
#define TASK2_PRIO 2
#define TASK3_PRIO 3
#define TASK4_PRIO 4
#define TASK5_PRIO 5 //MOD: prio changed to 6
#define TASK6_PRIO 6
#define TASKMAT_PRIO 7

OSStackType Stack1[STACK_SIZE];
OSStackType Stack2[STACK_SIZE];
OSStackType Stack3[STACK_SIZE];
OSStackType Stack4[STACK_SIZE];
OSStackType Stack5[STACK_SIZE];
OSStackType Stack6[STACK_SIZE];
OSStackType StackMatrix[STACK_SIZE];

queue *task6Q;
queue *taskMatrixQueue;

sem *s;




int main()
{

	LPC_GPIO1->FIODIR |= 1 << 18;
	LPC_GPIO1->FIODIR |= 1 << 20;
	LPC_GPIO1->FIODIR |= 1 << 21;
	LPC_GPIO1->FIODIR |= 1 << 23;

	//currentTCB = malloc(sizeof(TCB));
	//highestTCB = malloc(sizeof(TCB));
	/*this should be called somewhere else*/

	uKern_Init();
	/* create ur tasks */
	task_create(TASK1_PRIO, Task1, NULL, STACK_SIZE, &Stack1[STACK_SIZE - 1], NULL);
	task_create(TASK2_PRIO, Task2, NULL, STACK_SIZE, &Stack2[STACK_SIZE - 1], NULL);
	task_create(TASK3_PRIO, Task3, NULL, STACK_SIZE, &Stack3[STACK_SIZE - 1], NULL);
	task_create(TASK4_PRIO, Task4, NULL, STACK_SIZE, &Stack4[STACK_SIZE - 1], NULL);
	task_create(TASK5_PRIO, Task5, NULL, STACK_SIZE, &Stack5[STACK_SIZE - 1], NULL);
	task_create(TASK6_PRIO, Task6, NULL, STACK_SIZE, &Stack6[STACK_SIZE - 1], NULL);
	task_create(TASKMAT_PRIO, TaskMatrix, NULL, STACK_SIZE, &StackMatrix[STACK_SIZE - 1], NULL);

	/* buggy method. dynamic task allocation causes bugs, since we don't have heap reserved space */
	//task_create(TASK1_PRIO, Task1, NULL, STACK_MIN_SIZE);
	//task_create(TASK2_PRIO, Task2, NULL, STACK_MIN_SIZE);

	/*init other services*/

	task6Q = queueCreate(10, sizeof(uint_32));

	taskMatrixQueue = queueCreate(10, sizeof(uint_32));
	s = sem_init(1);

	StartOS();
	/* We actually do not expect to ever reach here */
}

//factorial of 12
void Task1(void *args)
{

	int i = 0;
	uint_32 fact = 1;
	while(1){
		fact = 1;
		for (i = 0; i < 5; i++){
			;;
		}
		for (i = 1; i < 10; i++)
			fact = fact*(i + 1);
		
		LPC_GPIO1->FIOPIN ^= 1 << 18;
		timeDelay(100);
	}
}


void TaskMatrix(void *args)
{
	unsigned char message[] = {0, 0, 0, 0, 0, 0x1F, 0x8, 0x4, 0x8, 0x1F, 0, 0x10, 0x8,\
			0x7, 0x8, 0x10, 0x0, 0x0, 0x0, 0x1F, 0x1, 0x1 , 0x0, 0x1F, 0x11, 0x1F, 0x0, 0x18, 0x6, 0x1, 0x6, 0x18, 0x0, 0x1F, 0x15, 0x15};


	int meslen = sizeof(message);
	unsigned short int phase = 0;
	dotmat_init();
	int cntr = 0;
	int fast = 0;
	uint_8 temp;

	while(1){

		unplot_col(fast);
		fast = (++fast) % MAT_MAX_COL;
		plot_col(fast);
		temp =  message[(fast + phase) % meslen] ;
		plot_val(temp);
		if((cntr++ % 250) == 0)
					phase = (phase + 1) % meslen;
		timeDelay(1);
	}

}
//prime numbas (stop after 10 first)
void Task2(void *args)
{
	int i,j;
	uint_32 primes[100];
	uint_8 found = 0;
	uint_8 CpuUtil = 0;
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
				//timeDelay( 200);//yield();//prioEnable(TASK1_PRIO);
			}
		}

		LPC_GPIO1->FIOPIN ^= 1 << 20;

		CpuUtil = getCpuUtilization();

		timeDelay( 200);
	}
}

/* fibonacci task */
void Task3(void *args)
{	
	
	uint_32 fibonacci[100];
	int i;
	while(1){
		fibonacci[0] = 0;
		fibonacci[1] = 1;
		for (i = 2; i < 100; i++)
			fibonacci[i] = fibonacci[i - 1] + fibonacci[(i - 2)];

		queueSendToTail(task6Q, (void *)&fibonacci[3]);
		//sem_release(s);
		timeDelay(100);
	}
}

void Task4(void *args){
	while(1){
		sem_get(s, 1000);
		LPC_GPIO1->FIOPIN ^= 1 << 21;
		/*timeDelay(1000);*/
	}
}

void Task5(void *args){
	while(1){
		timeDelay(2000);
		yield();
	}
}

void Task6(void *args){
	uint_32 *buff = portMalloc(sizeof(uint_32));

	while(1){

		queueReceive(task6Q, buff, 0);
		LPC_GPIO1->FIOPIN ^= 1 << 23;
		/*timeDelay(100);*/
	}
}

#if IDLE_TASK_HOOK == 1
void idleTaskHook()
{
	/* user defiend code here*/
	/*do sth like this*/
	/*if (sleepOnIdle){
		sleep
	}*/
	;;

}
#endif
