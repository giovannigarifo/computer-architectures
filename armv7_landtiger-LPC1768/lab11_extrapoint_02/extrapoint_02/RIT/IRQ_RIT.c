/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../button_EXINT/button.h"
#include "../dac/dac.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;

/*----------------------------------------------------------------------------
  WHICH PUSHBUTTON HAS BEEN PRESSED
 *----------------------------------------------------------------------------*/
extern unsigned char PUSHBUTTON_TYPE;


int down=0;

void RIT_IRQHandler (void)
{					
	static int rightJoystick=0;
	static int leftJoystick=0;
	
	static int blind_was_pressed;
	
	/* ----------------- */
	/* button management */
	if(down!=0 && LED_STATUS != CARYELLOW_PEDRED){
		
		/* KEY1 or KEY2 were pressed and are still pressed */
		if(((LPC_GPIO2->FIOPIN & (1<<11)) == 0 || (LPC_GPIO2->FIOPIN & (1<<12)) == 0) 
				 && PUSHBUTTON_TYPE == STANDARD_PUSHBUTTON)
			PUSHBUTTON_TYPE = STANDARD_PUSHBUTTON; //confirm
		/* INT0 was pressed and it's still pressed */
		else if((LPC_GPIO2->FIOPIN & (1<<10)) == 0 && PUSHBUTTON_TYPE == BLIND_PUSHBUTTON)
			PUSHBUTTON_TYPE = BLIND_PUSHBUTTON; //confirm
		/* A button type was pressed, but a different one is now pressed or no button is pressed at all */
		else
			PUSHBUTTON_TYPE = ERR_PUSHBUTTON; // go to the else and gracefully return
		
		/* Take action or exit if action already been performed */
		if(PUSHBUTTON_TYPE == STANDARD_PUSHBUTTON || PUSHBUTTON_TYPE == BLIND_PUSHBUTTON){
		
			down++;	//first time enters here down value is 1, increasing it to 2
			
			switch(down){
				
				case 2:	//button has been pushed for 50ms: push confirmed.
					
					// if BLIND_PUSHBUTTON was pressed start the confirmation sound
					if(PUSHBUTTON_TYPE == BLIND_PUSHBUTTON){
						
						init_timer(2, MATCH_LA_NOTE_TICK); //initialize to count 0,05ms
						enable_timer(2);
						blind_was_pressed = BLIND_PUSHBUTTON; // save the fact that the blind pushbutton has been pressed
					}
				
					BUTTON_pressed();
					break;
				
				default: //button is still pressed but action already performed
					break;
			}
			
		} else {	
				/* button released or wrong button pressed (with respect to the previous test): reset button behaviour */
				down=0;			
			
				// if BLIND_PUSHBUTTON was the pressed button, now it have been released: stop the sound
				if(blind_was_pressed == BLIND_PUSHBUTTON){
					reset_timer(2);
					disable_timer(2);
					DAC_reset();
					blind_was_pressed = ERR_PUSHBUTTON; // reset state, no pushbutton pressed at all
					PUSHBUTTON_TYPE = ERR_PUSHBUTTON; // reset state, so that timer1 start again toggling the sound if it should 
				}
					
				//reset buttons
				NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
				
				NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
				
				NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
		
	/* ------------------- */
	/* joystick management */
	
	//right joystick
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && LED_STATUS == CARRED_PEDGREEN){	/* Joytick Right pressed and initial status: enter maintenance mode */
		
		rightJoystick++;
		
		switch(rightJoystick){
			case 1:
				//setup maintenance mode
				LED_STATUS = MAINTENANCE;
			
				//disable timer0
				reset_timer(0);
				disable_timer(0);
			
				// initiale and enable TIMER1: loudspeaker 1s on 1s off, CarYellow and PedRed Led flashing
				reset_timer(1);
				init_timer(1,MATCH_1/TIMER_DEBUG_SCALER);
				enable_timer(1);
			
				/* enable ADC and start first conversion */
				ADC_enable();
				ADC_start_conversion();	//the ADC starts to convert values obtained by the potentiometer
			
				break;
			default:
				break;
		}
	}
	else{ //right joystick no longer pressed
			rightJoystick = 0;
	}
	
	//left joystick
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && LED_STATUS == MAINTENANCE){	/* Joytick left pressed: if in maintenance mode, exit */
		
		leftJoystick++;
		
		switch(leftJoystick){
			case 1:
				//restore initial mode for leds
				LED_STATUS = CARRED_PEDGREEN;
				LED_CarRed_PedGreen();
			
				// disable ADC (power down)
				ADC_stop_conversion();
				ADC_disable();
				
				//reset dac
				DAC_reset();
			
				// start normal sempahore transition between states
				enable_timer(0);
				init_timer(0,MATCH_15/TIMER_DEBUG_SCALER);
			
				// initiale and enable TIMER1: loudspeaker 1s on 1s off, required by status CARRED_PEDGREEN
				enable_timer(1);	
				init_timer(1,MATCH_1/TIMER_DEBUG_SCALER);
				
				
			
				break;
			default:
				break;
		}
	}
	else{ //right joystick no longer pressed
			leftJoystick = 0;
	}
	
	//ADC: if we're in the maintenace mode, at every polling rate require a conversion to the ADC
	if(LED_STATUS == MAINTENANCE)
		ADC_start_conversion();	//the ADC starts to convert values obtained by the potentiometer, it produces an interrupt when the conversion is finished

	
		
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
