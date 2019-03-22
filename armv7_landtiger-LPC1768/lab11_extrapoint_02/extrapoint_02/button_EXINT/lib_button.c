#include "button.h"
#include "lpc17xx.h"
#include "../timer/timer.h"
#include "../led/led.h"

/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;

/*----------------------------------------------------------------------------
  KEEP INFO IF PUSHBUTTON HAS ALREADY BEEN PRESSED ONE TIME
 *----------------------------------------------------------------------------*/
unsigned char PUSHBUTTON_CROSSING = NOT_REQUESTED;
unsigned char PUSHBUTTON_TYPE;

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {

  LPC_PINCON->PINSEL4    |= (1 << 20);		 /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 10);    /* PORT2.10 defined as input          */

  LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 11);    /* PORT2.11 defined as input          */
  
  LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 12);    /* PORT2.12 defined as input          */

  LPC_SC->EXTMODE = 0x7;

  NVIC_EnableIRQ(EINT2_IRQn);              /* enable irq in nvic                 */
	NVIC_SetPriority(EINT2_IRQn, 1);				 /* priority, the lower the better     */
  NVIC_EnableIRQ(EINT1_IRQn);              /* enable irq in nvic                 */
	NVIC_SetPriority(EINT1_IRQn, 2);				 
  NVIC_EnableIRQ(EINT0_IRQn);              /* enable irq in nvic                 */
	NVIC_SetPriority(EINT0_IRQn, 3);				 /* decreasing priority	from EINT2->0	 */
}


/*----------------------------------------------------------------------------
	PEDESTRIAN CROSSING REQUESTED
 *----------------------------------------------------------------------------*/
void BUTTON_pressed(){
		
	if (LED_STATUS == CARRED_PEDGREEN){ //reset timer, remain in status 
		
		reset_timer(0); //clear timer registers
		init_timer(0,MATCH_15/TIMER_DEBUG_SCALER); //initialize to count 15 seconds
		enable_timer(0);
		
	} else if (LED_STATUS == CARRED_PEDFLASHGREEN){ //transition to CARRED_PEDGREEN
		
		LED_STATUS = CARRED_PEDGREEN; 
		
		reset_timer(0);
		init_timer(0,MATCH_15/TIMER_DEBUG_SCALER); //initialize timer0 to count 15 seconds
		enable_timer(0);
		
		// initiale and enable TIMER1: loudspeaker 1s on 1s off
		init_timer(1,MATCH_1/TIMER_DEBUG_SCALER);
		enable_timer(1);
		
		LED_CarRed_PedGreen(); //change led value
	
	} else if (LED_STATUS == CARGREEN_PEDRED){
		
		if(PUSHBUTTON_CROSSING == NOT_REQUESTED){	//if the button has not already been pressed
			init_timer(0,MATCH_5/TIMER_DEBUG_SCALER); //initialize timer0 to count 5 seconds
			enable_timer(0);
			PUSHBUTTON_CROSSING = ALREADY_REQUESTED;
		}
	
	} else if (LED_STATUS == CARYELLOW_PEDRED){
		//nop
	}

}
