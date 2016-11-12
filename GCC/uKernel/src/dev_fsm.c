/* states */
#define NORMAL              0x10

#define GET                 0x20
#define CPU_UTIL            0x21

#define SET                 0x30
#define SET_LED_VAL         0x31

/* commands */
/* get commands */
#define GET_CMD             0xE0
#define GET_CPU_UTIL_CMD    0xEF
/* set commands */
#define SET_CMD             0xD0        /* SET_CMD                              followed by: */
#define SET_LED_VAL         0xDF        /* ---- SET_LED_VAL                     followed by: */
#define SET_TRIGGER_TCS     0xDE        /* ---- SET_TRIGGER_TCS                 followed by: */
#define SET_TRIGGER_TC_ALL  0xFF        /*      ---- SET_TRIGGER_TC_ALL         or:          */
                                        /*      ---- test case number (8 bit)                */
#define SET_TASK_ENABLE     0xDC        /* ---- SET_TASK_ENABLE                 followed by: */
                                        /*      ---- task priority                           */


/* dummy cmd - just for spi reading */
#define DUMMY_CMD           0xFFU

uint32_t state_machine = NORMAL;
uint32_t cntr;



void fsm_cyclic(uint8_t cmd)
{
#ifndef TEST_MESSAGE_QUEUE_SPI
     if (state_machine == NORMAL) {
	  
	  if(cmd == SET_CMD) {
	       state_machine = SET;
	       tx_data = cmd;
	       spi_data.tx_data = (void *)&tx_data;
	       
	  }else if(cmd == GET_CMD) {
	       state_machine = GET;
	       tx_data = cmd;
	       spi_data.tx_data = (void *)&tx_data;
	  }
     } else if (state_machine == SET) {
	  if(cmd == SET_LED_VAL) { 
	       LPC_GPIO1->FIOPIN &= ~(1 << 18);
	       LPC_GPIO1->FIOPIN &= ~(1 << 20);
	       LPC_GPIO1->FIOPIN &= ~(1 << 21);
	       LPC_GPIO1->FIOPIN &= ~(1 << 23);
	       
	       if(cmd & (1 << 0)) {
		    LPC_GPIO1->FIOPIN |= 1 << 18;
	       }
	       if(cmd & (1 << 1)) {
		    LPC_GPIO1->FIOPIN |= 1 << 20;
	       }
	       if(cmd & (1 << 2)) {
		    LPC_GPIO1->FIOPIN |= 1 << 21;
	       }
	       if(cmd & (1 << 3)) {
		    LPC_GPIO1->FIOPIN |= 1 << 23;
	       }
	       state_machine = NORMAL;
	  }
	  
	  
	  
     } else if (state_machine == GET) {
	  if(cmd == GET_CPU_UTIL_CMD) {
	       state_machine = CPU_UTIL;
	       cntr = 4;
	       tx_data = 4;
	       spi_data.tx_data = (void *)&tx_data;
	  }
	  
	  
     } else if (state_machine == CPU_UTIL) {
	  /* lock_cpu util array */
	  if (cmd == DUMMY_CMD) {
	       if (cntr > 0) {
		    cntr = cntr - 1;
		    tx_data = cpuLoadArr[cntr];
		    spi_data.tx_data = (void *)&tx_data;
	       } else {
		    state_machine = NORMAL;
		    /* unlock cpu util array */
	       }
	  }
     }
     
     
     #else

     if((cmd) != pre_val + 1) {
	  LPC_GPIO1->FIOPIN |=  1 << 18;
     } else {
	  pre_val = (cmd % 5);
     }
     
     queue_sendToTail(task8Q, (void *)&cmd);
     #endif
     /* TODO: */
     /* EXIT_CRITICAL(); */
     /* process the request:
      *  if read your id then write back your id
      *  if not read your id then process the command you read:
      *     - send back data according to what you read
      *     - perform an action according to what you read and write back E_OK/E_NOT_OK
      */
     /* call SPI_ReadWrite(LPC_SPI, , SPI_TRANSFER_INTERRUPT);*/
     if(LPC_SPI->SPINT & SPI_SPINT_INTFLAG){
	  LPC_SPI->SPINT = SPI_SPINT_INTFLAG;
     }

     SPI_ReadWrite(LPC_SPI, &spi_data, SPI_TRANSFER_INTERRUPT);
}
