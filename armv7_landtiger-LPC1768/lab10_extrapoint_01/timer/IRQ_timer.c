/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../button_EXINT/button.h"

/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;


/*----------------------------------------------------------------------------
  KEEP INFO IF PUSHBUTTON HAS ALREADY BEEN PRESSED ONE TIME
 *----------------------------------------------------------------------------*/
extern unsigned char PUSHBUTTON_CROSSING;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	
	if (LED_STATUS == CARRED_PEDGREEN){ //transition to CARRED_PEDFLASHGREEN
		
		LED_STATUS = CARRED_PEDFLASHGREEN; 
		
		init_timer(0, MATCH_5/TIMER_DEBUG_SCALER); //initialize to count 5 seconds
		enable_timer(0);
		
		//start pedestrian semaphore flashing: TIMER1
		init_timer(1,MATCH_05/TIMER_DEBUG_SCALER);
		enable_timer(1);
	
	} else if (LED_STATUS == CARRED_PEDFLASHGREEN){ //transition to CARGREEN_PEDRED
		
		LED_STATUS = CARGREEN_PEDRED;
		
		// stop pedestrian semaphore flashing
		disable_timer(1);
		reset_timer(1);
		
		LED_CarGreen_PedRed();
	
	} else if (LED_STATUS == CARGREEN_PEDRED){ //transition to CARYELLOW_PEDRED
		
		LED_STATUS = CARYELLOW_PEDRED;
		
		PUSHBUTTON_CROSSING = NOT_REQUESTED; //reset value
		
		init_timer(0, MATCH_5/TIMER_DEBUG_SCALER); //initialize to count 5 seconds
		enable_timer(0);

		LED_CarYellow_PedRed();
	
	} else if(LED_STATUS == CARYELLOW_PEDRED){ //transition to CARRED_PEDGREEN
		
		LED_STATUS = CARRED_PEDGREEN;
		
		init_timer(0, MATCH_15/TIMER_DEBUG_SCALER); //initialize to count 15 seconds
		enable_timer(0);
	
		LED_CarRed_PedGreen();		
	}
  
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	static unsigned char blink = 0;
	
	//switch pedestrian semaphore color
	if(blink == 0){
		
		LED_CarRed_PedOff();
		blink = 1;
		
	} else if( blink == 1){
		
		LED_CarRed_PedGreen();
		blink = 0;
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
