/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Atomic led init functions
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "led.h"

/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
unsigned char LED_STATUS;



/*----------------------------------------------------------------------------
  Function that initializes LEDs 
 *----------------------------------------------------------------------------*/
void LED_init(void) {
	
	//set status
	LED_STATUS = LED_ALL_OFF;

	//set PIN mode to GPIO (00) for P2.0 to P2.7, the MSBits to 1 preserve the other values
  LPC_PINCON->PINSEL4 &= 0xFFFF0000;	
	
	LPC_GPIO2->FIODIR   |= 0x000000FF;  //P2.0...P2.7 Output (LEDs on PORT2 defined as Output, P2.0=LD11, P2.7=LD4)
}

void LED_deinit(void) {

  LPC_GPIO2->FIODIR &= 0xFFFFFF00;  //P2.0...P2.7 Output LEDs on PORT2 defined as Output
}
