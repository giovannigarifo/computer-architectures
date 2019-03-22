#include "button.h"
#include "lpc17xx.h"
#include "../timer/timer.h"
#include "../led/led.h"

/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;


/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {

	// init button INT0, connected to pin P2.10
  LPC_PINCON->PINSEL4    |= (1 << 20);		 /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 10);    /* P2.10 defined as input          */

	// init button KEY1, connected to pin 2.11
  LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 11);    /* P2.11 defined as input          */
  
	// init button KEY2, connected to pin 2.12
  LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 12);    /* P2.12 defined as input          */

  LPC_SC->EXTMODE = 0x7;

	/* Interrupt setup: interrupt priority here*/
	
	//highest priority
  NVIC_EnableIRQ(EINT2_IRQn);              /* enable irq in nvic  */
	NVIC_SetPriority(EINT2_IRQn, 1);				 /* priority, the lower the value the highest priority */
	
  NVIC_EnableIRQ(EINT1_IRQn);              /* enable irq in nvic  */
	NVIC_SetPriority(EINT1_IRQn, 2);				 /* priority, the lower the value the highest priority */
	
  NVIC_EnableIRQ(EINT0_IRQn);              /* enable irq in nvic  */
	NVIC_SetPriority(EINT0_IRQn, 3);				 /* priority, the lower the value the highest priority  */
	//lowest priority
}


/*----------------------------------------------------------------------------
	PEDESTRIAN CROSSING REQUESTED
 *----------------------------------------------------------------------------*/
void BUTTON_pressed(){
		
}
