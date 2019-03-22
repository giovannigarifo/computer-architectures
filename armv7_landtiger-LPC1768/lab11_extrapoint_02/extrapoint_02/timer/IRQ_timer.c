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
#include "../dac/dac.h"
#include "../button_EXINT/button.h"

/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;


/*----------------------------------------------------------------------------
  KEEP INFO IF PUSHBUTTON HAS ALREADY BEEN PRESSED ONE TIME
 *----------------------------------------------------------------------------*/
extern unsigned char PUSHBUTTON_CROSSING;

/*----------------------------------------------------------------------------
  WHICH PUSHBUTTON HAS BEEN PRESSED
 *----------------------------------------------------------------------------*/
extern unsigned char PUSHBUTTON_TYPE;


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler, it handles the transition
**									between the states of the semaphore.
**
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	
	if (LED_STATUS == CARRED_PEDGREEN){ //transition to CARRED_PEDFLASHGREEN
		
		LED_STATUS = CARRED_PEDFLASHGREEN; 
		
		//stop loudspeaker sound 1s on and 1s off
		reset_timer(2);
		disable_timer(2);
		DAC_reset(); // 0V to loudspeaker
		
		reset_timer(0);
		init_timer(0, MATCH_5/TIMER_DEBUG_SCALER); //initialize to count 5 seconds
		enable_timer(0);
		
		//start pedestrian semaphore flashing with loud speaker: TIMER1
		init_timer(1,MATCH_05/TIMER_DEBUG_SCALER);
		enable_timer(1);
	
	} else if (LED_STATUS == CARRED_PEDFLASHGREEN){ //transition to CARGREEN_PEDRED
		
		LED_STATUS = CARGREEN_PEDRED;
		
		// stop pedestrian semaphore flashing
		reset_timer(1);
		disable_timer(1);
		
		//stop loudspeaker sound (it may be already stopped)
		reset_timer(2);
		disable_timer(2);
		DAC_reset(); // 0V to loudspeaker
		
		LED_CarGreen_PedRed();
	
	} else if (LED_STATUS == CARGREEN_PEDRED){ //transition to CARYELLOW_PEDRED
		
		LED_STATUS = CARYELLOW_PEDRED;
		
		PUSHBUTTON_CROSSING = NOT_REQUESTED; //reset value
		
		reset_timer(0);
		init_timer(0, MATCH_5/TIMER_DEBUG_SCALER); //initialize to count 5 seconds
		enable_timer(0);

		LED_CarYellow_PedRed();
	
	} else if(LED_STATUS == CARYELLOW_PEDRED){ //transition to CARRED_PEDGREEN
		
		LED_STATUS = CARRED_PEDGREEN;
		
		reset_timer(0);
		init_timer(0, MATCH_15/TIMER_DEBUG_SCALER); //initialize to count 15 seconds
		enable_timer(0);
		
		// initiale and enable TIMER1: loudspeaker 1s on 1s off
		init_timer(1,MATCH_1/TIMER_DEBUG_SCALER);
		enable_timer(1);
	
		LED_CarRed_PedGreen();		
	}
  
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**									Used for flashing led and sound
**		
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	static unsigned char blink = 0;
	
	//if we're in this status and there blind pushbutton ISN'T currently pressed
	if( LED_STATUS == CARRED_PEDGREEN && PUSHBUTTON_TYPE != BLIND_PUSHBUTTON) //loud speaker sound on/off every 1s
	{ 

		//switch sound on and off
		if(blink == 0){
		
			blink = 1;
		
			//start loudspeaker sound
			init_timer(2, MATCH_LA_NOTE_TICK); //initialize to count 0,05ms
			enable_timer(2);
		
		} else if( blink == 1){
		
			blink = 0;
		
			//stop loudspeaker sound
			reset_timer(2);
			disable_timer(2);
			DAC_reset(); // 0V to loudspeaker
		}
	} 
	else if ( LED_STATUS == CARRED_PEDFLASHGREEN) //flashing green with loud speaker
	{
		//switch pedestrian semaphore color
		if(blink == 0){
		
			LED_CarRed_PedOff();
			blink = 1;
		
			//stop loudspeaker sound, but do not if the BLIND pushbutton is currently pressed
			if(PUSHBUTTON_TYPE != BLIND_PUSHBUTTON){
				reset_timer(2);
				disable_timer(2);
				DAC_reset(); //0V to loudspeaker	
			}
			
		} else if( blink == 1){
		
			LED_CarRed_PedGreen();
			blink = 0;
		
			//start loudspeaker sound
			if(PUSHBUTTON_TYPE != BLIND_PUSHBUTTON){
				init_timer(2, MATCH_LA_NOTE_TICK); //initialize to count 0,05ms
				enable_timer(2);
			}
		}
	}
	else if (LED_STATUS == MAINTENANCE)
	{
	
		//switch pedestrian and car semaphore color
		if(blink == 0){
		
			LED_CarOff_PedOff();
			blink = 1;
		
			//stop loudspeaker sound
			reset_timer(2);
			disable_timer(2);
			DAC_reset(); //0V to loudspeaker
		
		} else if( blink == 1){
		
			LED_CarYellow_PedRed();
			blink = 0;
		
			//start loudspeaker sound
			init_timer(2, MATCH_LA_NOTE_TICK); //initialize to count 0,05ms
			enable_timer(2);
		}
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler: use to feed the 
**									sinusoid to the loudspeaker so that it reproduce the note
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
	/* DAC management: feed a note tick to the loudspeaker */	
	DAC_feed_loudspeaker();

  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
