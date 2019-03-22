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

extern int count_negative_and_odd(int* array, int len);

/*----------------------------------------------------------------------------
  Number of VARs to be recorded in VETT
 *----------------------------------------------------------------------------*/
#define N 1
#define	BLINK_ON 1
#define BLINK_OFF 0
#define	NO_BLINK 2

/*----------------------------------------------------------------------------
  RIT INTERRUPT HANDLER
 *----------------------------------------------------------------------------*/
void RIT_IRQHandler (void)
{					
	static int upJoystick=0;
	static int downJoystick=0;
	static int selJoystick=0;
	
	static signed int VAR1 = 0;
	static int VETT[N] = {0};
	static int vettPos = 0;
	
	//blinking 
	static unsigned char blink = NO_BLINK; //at reset no LD4 blinking
	static unsigned char blink_on_cnt = 0;
	static unsigned char blink_off_cnt = 0;
	
	/* ------------------- */
	/* blinking management */
	
	// 1.3s <=> 26 RIT interruptions
	// 0.9s <=> 18 RIT interruptions
	if(blink == BLINK_ON)
		blink_on_cnt++;
	else if (blink == BLINK_OFF)
		blink_off_cnt++;
		
	if(blink_on_cnt >=26)
	{
		LED_Off(7);
		blink_on_cnt = 0;
		blink = BLINK_OFF; //change blinking state
	}
	else if(blink_off_cnt >= 18)
	{
		LED_On(7);
		blink_off_cnt = 0;
		blink = BLINK_ON; //change blinking state
	}
	
	/* ------------------- */
	/* joystick management */
	
	// - SEL joystick - 
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	/* Joytick Selection pressed */
		
		selJoystick++;
		
		switch(selJoystick){
			case 1:
				// confirm the current value of VAR1
				
				//copy into free position of VETT
				VETT[vettPos] = VAR1;
				
				//be ready for next VAR
				VAR1 = 0;
				vettPos += 1;
			
				//test if vector is now full, if yes take ASM action
				if (vettPos == N)
				{			
					//call asm function: count the number of "negative and odd" numbers in VETT
					int res = count_negative_and_odd(VETT, vettPos);
					
					//restore initial state without any blinking or led on
					LED_Out(0);
					LED_Off(7);
					blink = NO_BLINK;
					blink_on_cnt = 0;
					blink_off_cnt = 0;
					
					//setup leds accordingly to the returned value
					if(res == 0)
					{
							//blink LD04 1.3s on, 0.9s off
							blink = BLINK_ON;
							LED_On(7); //turn on LD4
					}
					else
					{
						//LD05 to LD11 show binary representation of result: EXCLUDE LD04
						res &= 0x7F; //select only the least significant 7 bits (we only have 8 leds, and LD4 cannot be used)												
						LED_Out(res);
					}
						
					//reset data structure for next operation
					//VETT is just overwritten in next operations
					vettPos = 0;
				}
				
				break;
			default:
				break;
		}
	}
	else{ //sel joystick no longer pressed
			selJoystick = 0;
	}
	
	// - DOWN joystick - 
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	/* Joytick Down pressed */
		
		downJoystick++;
		
		switch(downJoystick){
			case 1:
				//substract 7 to VAR1
				VAR1 = VAR1 - 7;
				break;
			default:
				break;
		}
	}
	else{ //down joystick no longer pressed
			downJoystick = 0;
	}
	
	// - UP joystick - 
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	/* Joytick Up pressed */
		
		upJoystick++;
		
		switch(upJoystick){
			case 1:
				//add 12 to VAR1
				VAR1 = VAR1 + 12;
				break;
			default:
				break;
		}
	}
	else{ //up joystick no longer pressed
			upJoystick = 0;
	}
	
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
