#include "LPC17xx.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_spi.h"
#include "lpc17xx_uart.h"

#include "heap.h"
#include "kernel.h"
#include "message_queue.h"
#include "sem.h"
#include "stats.h"
#include "task.h"

/* prios increase from 0 to 32*/
#define TASK1_PRIO 2
#define TASK2_PRIO 3
#define TASK3_PRIO 4
#define TASK4_PRIO 5
#define TASK5_PRIO 6
#define TASK6_PRIO 7
#define TASK7_PRIO 8
#define TASK8_PRIO 9

queue *task6Q;
queue *task8Q;
sem *s;
sem *task1_factorial;
uint_32 fact_result;
LPC_UART_TypeDef *UARTx;
uint8_t rxBuf[16] = {0};
uint32_t rxCnt = 0;
volatile Bool rxFlag = FALSE;

OSStackType Stack1[STACK_SIZE];
OSStackType Stack2[STACK_SIZE];
OSStackType Stack3[STACK_SIZE];
OSStackType Stack4[STACK_SIZE];
OSStackType Stack5[STACK_SIZE];
OSStackType Stack6[STACK_SIZE];
OSStackType Stack7[STACK_SIZE];
OSStackType Stack8[STACK_SIZE];

void PIN_Init(void);
void UART3_Init(void);
void init_uart(void);
void UART3_IRQHandler(void);
void SPI_IRQHandler(void);

void Task1(void *args);
void Task2(void *args);
void Task3(void *args);
void Task4(void *args);
void Task5(void *args);
void Task6(void *args);
void Task7(void *args);
void Task8(void *args);

#define NORMAL 0x1
#define GET 0x2
#define GET_CMD 0xFD
#define CPU_UTIL 0x21
#define GET_CPU_UTIL_CMD 0xFCU
#define SET 0x3
#define SET_CMD 0xFEU
#define DUMMY_CMD 0xFFU

uint32_t state_machine = NORMAL;
uint32_t cntr;

SPI_DATA_SETUP_Type spi_data;
uint_8 rx_data;
uint_8 tx_data;
uint_8 cpuLoadArr[4];
uint_32 temp;
uint_32 pre_val = 0;
int irqcntr = 0;
/* spi interrupt handler */
void SPI_IRQHandler(void) {
    /* LPC_GPIO1->FIOPIN ^= (1 << 18); */
    /* LPC_GPIO1->FIOPIN ^= (1 << 20); */
    /* LPC_GPIO1->FIOPIN ^= (1 << 21); */
    /* LPC_GPIO1->FIOPIN ^= (1 << 23); */
    interruptEnter();
    uint_32 ltemp = LPC_SPI->SPSR;
    if(ltemp & (1 << 3)) { LPC_GPIO0->FIOPIN |= (1 << 18); }
    if(ltemp & (1 << 4)) { LPC_GPIO0->FIOPIN |= (1 << 19); }
    if(ltemp & (1 << 5)) { LPC_GPIO0->FIOPIN |= (1 << 18); LPC_GPIO0->FIOPIN |= (1 << 19);}
    if(ltemp & (1 << 6)) { LPC_GPIO0->FIOPIN |= (1 << 20); }
    irqcntr++;
/* just for clearing the interrupt flag */
    SPI_ClearIntPending(LPC_SPI);
    SPI_IntCmd(LPC_SPI, DISABLE);
    sem_release(s);
    interruptExit();
}

uint8_t rx_buffer[26];

int main() {
    SPI_CFG_Type spi_conf;
    PINSEL_CFG_Type PinCfg;

    /********** leds init **********/
    LPC_GPIO1->FIODIR |= 1 << 18;
    LPC_GPIO1->FIODIR |= 1 << 20;
    LPC_GPIO1->FIODIR |= 1 << 21;
    LPC_GPIO1->FIODIR |= 1 << 23;
    /********** leds init **********/

    /********** initialize spi **********/
    /* pin init */

    LPC_SC->PCONP |= 1 << 8;
    LPC_GPIO0->FIODIR |= 1 << 31;

    LPC_PINCON->PINSEL0 |= 0b11 << 30;
    LPC_PINCON->PINSEL1 |= 0b111111 << 0;

    /* LPC_PINCON->PINMODE0 |= 0b10 << 14; */
    /* PINSEL_ConfigPin(&PinCfg); */
    /* PinCfg.Pinnum    = UART3_RX; */
    /* PINSEL_ConfigPin(&PinCfg); */

    /* clock init
     * not relevant in slave mode
     */
    /* spi init */
    SPI_ConfigStructInit(&spi_conf);
    SPI_Init(LPC_SPI, &spi_conf);
    NVIC_EnableIRQ(SPI_IRQn);
    /* spi data setup */
    tx_data = (uint_8)0xDE;
    rx_data = (uint_8)0x00;
    spi_data.tx_data = (void *)&tx_data;
    spi_data.rx_data = (void *)&rx_buffer[0];
    spi_data.length = sizeof(rx_buffer);
    ENABLE_INTERRUPTS();


    /********** initialize spi **********/

    /* initalize rtOS */
    kernel_init();

    /* don't create any tasks before kernel_init()!!! */
    task_create(TASK1_PRIO, Task1, NULL, STACK_SIZE, &Stack1[STACK_SIZE - 1], NULL);
    task_create(TASK2_PRIO, Task2, NULL, STACK_SIZE, &Stack2[STACK_SIZE - 1], NULL);
    task_create(TASK3_PRIO, Task3, NULL, STACK_SIZE, &Stack3[STACK_SIZE - 1], NULL);
    task_create(TASK4_PRIO, Task4, NULL, STACK_SIZE, &Stack4[STACK_SIZE - 1], NULL);
    task_create(TASK5_PRIO, Task5, NULL, STACK_SIZE, &Stack5[STACK_SIZE - 1], NULL);
    task_create(TASK6_PRIO, Task6, NULL, STACK_SIZE, &Stack6[STACK_SIZE - 1], NULL);
    task_create(TASK7_PRIO, Task7, NULL, STACK_SIZE, &Stack7[STACK_SIZE - 1], NULL);
    /* task_create(TASK8_PRIO, Task8, NULL, STACK_SIZE, &Stack8[STACK_SIZE - 1], NULL); */

    /*init other OS services*/
    task6Q = queue_create(10, 16 * sizeof(uint_8));
    task8Q = queue_create(10, sizeof(uint_8));

    s = sem_init(0);
    task1_factorial = sem_init(1);

    /* Start the kernel */
    kernel_start();

    /* We actually do not expect to ever reach here */
    return 0;
}
#ifdef TASK_CREATOR
void TaskManager(void *args) {
    while (1) {
        /* creation command shall be received by spi and sent to this queue */
        queue_receive(taskCreatorQ, creator_buff, 0);
        /* creation command shall be analyzed: priority of task, stack size,  */
        /** TODO: if you want to also send the code of the task to be executed, you need to fix the
         *       heap
         * If the code is not specified then you will have to find some default functions to execute.
         */
        task_prioEnable(creator_buff & 0xFF);
    }
}
#endif
/* Testing message_queue.c */
void TaskTestCase1(void *args) {
    while (1) {
        ;
        ;
    }
}
void TaskQueueSender(void *args) {}
void TaskQueueReceiver(void *args) {}
void TaskTestCase2(void *args) {
    while (1) {
    }
}
void TaskTestCase3(void *args) {
    while (1) {
    }
}
void TaskTestCase4(void *args) {
    while (1) {
    }
}
void TaskTestCase5(void *args) {
    while (1) {
    }
}

void TaskTestCase6(void *args) {
    while (1) {
    }
}

/*factorial of 100*/
void Task1(void *args) {
    args = (void *)0;
    args = (void *)((uint_32)args + 1);
    int i = 0;
    uint_32 fact = 1;

    while (1) {
        fact = 1;

        for (i = 1; i < 10; i++) {
            fact = fact * (i + 1);
        }
        sem_get(task1_factorial, 0);
        fact_result = fact;
        sem_release(task1_factorial);

        time_delay(100);
    }
}

/* prime numbas (stop after 10 first) */

void Task2(void *args) {
    args = (void *)0;
    args = (void *)((uint_32)args + 1);
    int i = 0, j;
    uint_32 primes[100];
    uint_8 found = 0;
    int cntr = 0;
    while (1) {
        for (i = 0; i < 100; i++) {
            for (j = 2; j < i; j++) {
                found = 1;
                if (((i % j) == 0)) {
                    found = 0;
                    break;
                }
            }
            if (found) {
                primes[cntr++ % 100] = i;
                found = 0;
            }
        }
        primes[0];
        sem_get(task1_factorial, 0);
        fact_result = 0xAA;
        sem_release(task1_factorial);

        time_delay(200);
    }
}

/* fibonacci task */
uint32_t del;
void Task3(void *args) {
    args = (void *)0;
    args = (void *)((uint_32)args + 1);
    uint_32 fibonacci[100];
    uint_32 cntr = 0;
    int i;
    del = 500;
    while (1) {
        fibonacci[0] = 0;
        fibonacci[1] = 1;
        for (i = 2; i < 1000; i++) fibonacci[i % 100] = fibonacci[(i - 1) % 100] + fibonacci[(i - 2) % 100];

        time_delay(del);
        /* LPC_GPIO1->FIOPIN ^= 1 << 23; */
    }
}

/* SPI task */
void Task4(void *args) {
    err_t semaphore;
    int loc_cntr;
    SPI_ReadWrite(LPC_SPI, &spi_data, SPI_TRANSFER_INTERRUPT);
    while (1) {

        semaphore = sem_get(s, 0);
        /* LPC_GPIO1->FIOPIN ^= (1 << 18); */
        /* LPC_GPIO1->FIOPIN ^= (1 << 20); */
        LPC_GPIO1->FIOPIN ^= (1 << 23);
        int cntr = SPI_ReadWrite(LPC_SPI, &spi_data, SPI_TRANSFER_POLLING);


    }
}

void Task5(void *args) {
    args = (void *)0;
    uint_32 i = 0;
    float_32 cpuLoad;
    while (1) {
        time_delay(30);
        cpuLoad = stats_getCpuUtilization();

        ENTER_CRITICAL();
        if (state_machine != CPU_UTIL) {
            cpuLoadArr[0] = (uint_8)((*(uint_32 *)&cpuLoad & 0xFF000000) >> 24);
            cpuLoadArr[1] = (uint_8)((*(uint_32 *)&cpuLoad & 0x00FF0000) >> 16);
            cpuLoadArr[2] = (uint_8)((*(uint_32 *)&cpuLoad & 0x0000FF00) >> 8);
            cpuLoadArr[3] = (uint_8)((*(uint_32 *)&cpuLoad & 0x000000FF));
        }
        EXIT_CRITICAL();
    }
}

void Task6(void *args) {
    args = (void *)0;
    err_t queue_err;
    uint_8 *buff = heap_malloc(16 * sizeof(uint_8));
    uint_32 loc_rxCnt;
    /*addded sth here*/
    while (1) {
        /* } */

        queue_receive(task6Q, (void *)buff, 0);
        /* LPC_GPIO1->FIOPIN ^= 1 << 20; */

        /* /\* limit of TX FIFO is 16 bytes *\/ */
        UART_Send(UARTx, (uint_8 *)&buff[1], ((uint_32)buff[0]) & 0xFF, NONE_BLOCKING);
        time_delay(10);
    }
}

void Task7(void *args) {
    init_uart();

    while (1) {
        if (rxFlag == TRUE) {
            UART_Send(LPC_UART3, (uint8_t *)"hello word\n", sizeof("hello word\n"), NONE_BLOCKING);
        }

        time_delay(100);
    }
}
/*
 * the spi-slave task
 * asynchronous server of SPI commands
 */

uint_8 array_spi[5];
void Task8(void *args) {
    /* err_t queue_err; */
    /* uint_8 *buff = heap_malloc(sizeof(uint_8)); */
    /* uint32_t loc_sr; */
    /* uint32_t state_machine = NORMAL; */
    /* uint32_t cntr; */
    /* uint32_t i; */
    /* /\* from know on I can accept spi commands *\/ */

    /* /\* initiate slave-listen mode *\/ */
    /* cntr = 0; */
    /* SPI_ReadWrite(LPC_SPI, &spi_data, SPI_TRANSFER_INTERRUPT); */
    /* while (1) { */
    /*     /\* enter here for half bottom processing only *\/ */
    /*     /\* Message queue test/SPI Test case *\/ */
    /*     queue_receive(task8Q, (void *)buff, 0); */
    /*     array_spi[cntr] = *buff; */
    /*     spi_data.tx_data = (void *)&cntr; */
    /*     if (cntr >= 4) { */
    /*         for (i = 0; i < 4; i++) { */
    /*             if (array_spi[i + 1] != array_spi[i] + 1) { */
    /*                 LPC_GPIO1->FIOPIN |= 1 << 20; */
    /*             } else if (array_spi[i] != i + 1) { */
    /*                 LPC_GPIO1->FIOPIN |= 1 << 21; */
    /*             } else { */
    /*                 array_spi[i] = 0; */
    /*             } */
    /*         } */
    /*         array_spi[i] = 0; */
    /*         cntr = 0; */
    /*     } else { */
    /*         cntr = cntr + 1; */
    /*     } */

    /*     /\* if (state_machine == NORMAL) { *\/ */

    /*     /\*      if(*buff == 0xFEU) { *\/ */
    /*     /\* 	    state_machine = SET; *\/ */
    /*     /\* 	    tx_data = 0xFE; *\/ */
    /*     /\* 	    spi_data.tx_data = (void *)&tx_data; *\/ */

    /*     /\*      }else if(*buff == 0xFD) { *\/ */
    /*     /\* 	    state_machine = GET; *\/ */
    /*     /\* 	    tx_data = 0xFD; *\/ */
    /*     /\* 	    spi_data.tx_data = (void *)&tx_data; *\/ */

    /*     /\*      } *\/ */

    /*     /\* } else if (state_machine == SET) { *\/ */
    /*     /\*      LPC_GPIO1->FIOPIN &= ~(1 << 18); *\/ */
    /*     /\*      LPC_GPIO1->FIOPIN &= ~(1 << 20); *\/ */
    /*     /\*      LPC_GPIO1->FIOPIN &= ~(1 << 21); *\/ */
    /*     /\*      LPC_GPIO1->FIOPIN &= ~(1 << 23); *\/ */

    /*     /\*      if(*buff & (1 << 1)) { *\/ */
    /*     /\* 	    LPC_GPIO1->FIOPIN |= 1 << 18; *\/ */
    /*     /\*      } *\/ */
    /*     /\*      if(*buff & (1 << 2)) { *\/ */
    /*     /\* 	    LPC_GPIO1->FIOPIN |= 1 << 20; *\/ */
    /*     /\*      } *\/ */
    /*     /\*      if(*buff & (1 << 3)) { *\/ */
    /*     /\* 	    LPC_GPIO1->FIOPIN |= 1 << 21; *\/ */
    /*     /\*      } *\/ */
    /*     /\*      if(*buff & (1 << 4)) { *\/ */
    /*     /\* 	    LPC_GPIO1->FIOPIN |= 1 << 23; *\/ */
    /*     /\*      } *\/ */
    /*     /\*      state_machine = NORMAL; *\/ */

    /*     /\* } else if (state_machine == GET) { *\/ */
    /*     /\*      if(*buff == CPU_UTIL) { *\/ */
    /*     /\* 	    cntr = 4; *\/ */
    /*     /\* 	    tx_data = ((uint_32)stats_getCpuUtilization() >> (cntr * 8)) & 0xFF; *\/ */
    /*     /\* 	    spi_data.tx_data = (void *)&tx_data; *\/ */
    /*     /\*      } else if (*buff == 0xFF) { *\/ */

    /*     /\* 	    if (cntr > 0) { *\/ */
    /*     /\* 		 cntr--; *\/ */
    /*     /\* 		 tx_data = ((uint_32)stats_getCpuUtilization() >> (cntr * 8)) & 0xFF; *\/ */
    /*     /\* 		 spi_data.tx_data = (void *)&tx_data; *\/ */
    /*     /\* 	    } else { *\/ */
    /*     /\* 		 state_machine = NORMAL; *\/ */
    /*     /\* 	    } *\/ */
    /*     /\*      } *\/ */
    /*     /\* } *\/ */
    /*     /\* read sospsr *\/ */
    /*     loc_sr = LPC_SPI->SPSR; */
    /*     if (loc_sr & (1 << 3) & (1 << 4) & (1 << 5) & (1 << 6)) { */
    /*         del = 100; */
    /*     } */
    /* } */
    
}

void init_uart(void) {
    /** System init */

    unsigned int tmp, i;

    /** UART init */
    PIN_Init();
    UART3_Init();
}
void PIN_Init(void) {
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

void UART3_Init(void) {
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
    UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV2; /* 8 Bytes */
    /* Initialize FIFO for UART3 peripheral */
    UART_FIFOConfig(UARTx, &UARTFIFOConfigStruct);

    /** config and enable interrupter */
    UART_IntConfig(UARTx, UART_INTCFG_RBR, ENABLE);

    /** NVIC enable interrupter , it's requesté”›ï¿½*/
    NVIC_EnableIRQ(SPI_IRQn);

    /*  Enable UART Transmit */
    UART_TxCmd(UARTx, ENABLE);
}

void UART3_IRQHandler(void) {
    uint8_t ReceiveValue;
    uint8_t i;
    uint32_t IIRValue;
    uint32_t loc_rxCnt;

    interruptEnter();

    loc_rxCnt = 0;
    IIRValue = UART_GetIntId(LPC_UART3);

    while ((IIRValue & 0x01) == 0) {
        switch (IIRValue & 0x0E) { /* flag */
            case UART_IIR_INTID_RDA:
                for (i = 0; i < 8; i++) {
                    /* rxFlag = TRUE; */
                    ReceiveValue = UART_ReceiveByte(LPC_UART3);
                    rxBuf[1 + loc_rxCnt++] = ReceiveValue;
                }
                ENTER_CRITICAL();
                rxBuf[0] = loc_rxCnt;
                queue_sendToTail(task6Q, (void *)&rxBuf[0]);
                EXIT_CRITICAL();
                /* LPC_GPIO1->FIOPIN ^= 1 << 18; */
                break;
            case UART_IIR_INTID_CTI:
                /* rxFlag = TRUE; */
                ReceiveValue = UART_ReceiveByte(LPC_UART3);
                rxBuf[1 + loc_rxCnt++] = ReceiveValue;
                ENTER_CRITICAL();
                rxBuf[0] = loc_rxCnt;
                queue_sendToTail(task6Q, (void *)&rxBuf[0]);
                EXIT_CRITICAL();
                /* LPC_GPIO1->FIOPIN ^= 1 << 18; */
                break;

            default:
                break;
        }
        IIRValue = UART_GetIntId(LPC_UART3);
    } /* if have another interrupter hand on */
    interruptExit();
}
