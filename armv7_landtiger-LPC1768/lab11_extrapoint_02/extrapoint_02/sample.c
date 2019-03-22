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

extern unsigned char LED_STATUS;

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
	DAC_init();														/* DAC initialization 								*/
	ADC_init();														/* ADC initialization 								*/
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec, hex=50ms*100MHz    */
	enable_RIT(); 												/* RIT enabled, start counting, used for polling and debouncing	*/
	
	
	/* ---- */
	/* set initial state CARRED_PEDGREEN */
	LED_STATUS = CARRED_PEDGREEN;
	LED_CarRed_PedGreen();
	LED_STATUS = CARRED_PEDGREEN;
	
	// initialize and enable TIMER0: 
	init_timer(0,MATCH_15/TIMER_DEBUG_SCALER);
	enable_timer(0);
	
	// initiale and enable TIMER1: loudspeaker 1s on 1s off
	init_timer(1,MATCH_1/TIMER_DEBUG_SCALER);
	enable_timer(1);
	
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
