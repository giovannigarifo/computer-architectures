/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  
	/* 
	*	System and Periphral initialization.
	*	Only actually used peripheral are initialized, for better power efficiency.
	*/

	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	
	// initialize and enable TIMER0: change state
	init_timer(0,MATCH_15/TIMER_DEBUG_SCALER);
	enable_timer(0);
	
	/* 
	*	Power mode selection: power-down
	*/
	LPC_SC->PCON |= 0x1;									/* power-down	mode: PM1,PM0 bits of PCON = 01*/
	LPC_SC->PCON &= 0xFFFFFFFFD;						
		
  while (1) {       
		__ASM("wfi");	// execute WFI to enter selected power mode
  }

}
