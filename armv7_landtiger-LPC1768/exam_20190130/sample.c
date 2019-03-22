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
#include "RIT/RIT.h"
#include "dac/dac.h"
#include "adc/adc.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  
	/* 
	*	System and Periphral initialization.
	*	Only actually used peripheral are initialized, for better power efficiency.
	*/

	//SYS INIT
	SystemInit(); 	/* System Initialization (i.e., PLL) */
  
	//LEDS
	LED_init();   	/* LED Initialization  */
  
	//RIT
	init_RIT(0x004C4B40);		/* RIT Initialization 50 msec, hex=50ms*100MHz    */
	
	/* ---- */
	/* Main */
	
	//energy efficiency, they're tunred on at system startup
	powerdown_timer(0);
	powerdown_timer(1);
	
	enable_RIT(); 	/* RIT enabled: handles joystick polling, button debouncing	*/

	/* ---- */
	
	/* 
	*	Power mode selection: power-down
	*/
	LPC_SC->PCON |= 0x1;									/* power-down	mode: PM1,PM0 bits of PCON = 01*/
	LPC_SC->PCON &= 0xFFFFFFFFD;						
		
  while (1) {       
		__ASM("wfi");	// execute WFI to enter selected power mode
  }

}
