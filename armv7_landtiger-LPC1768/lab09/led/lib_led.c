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
  Function that initializes LEDs and switch them off
 *----------------------------------------------------------------------------*/

unsigned char led_value;
unsigned char on_led
	; //led that is currently tuned on


void LED_init(void) {

	// PINSEL4 register controls the mode of each GPIO PIN, from P2.0 to 0.13.
	// for each PIN, a couple of bits in PINSEL4 is used to express is mode of operation.
	// here we set the mode of operation of the pin2.0 to pin2.7 to GPIO forcing a "00" value for each 
	// couple of bits.
  LPC_PINCON->PINSEL4 &= 0xFFFF0000;	//PIN mode GPIO (00b value per P2.0 to P2.7)
	
	// here we force the direction of the GPIO: each GPIO pin can be threated as input or as output, 
	// in our case we do an OR with previous value to preserve the direction of all pins, but we force the LSB
	// regarding the pin from P2.0 to PIN2.7 to 1, forcing their mode of operation to output (they're connected
  // to LEDs, so cannot be nothing more)
	LPC_GPIO2->FIODIR   |= 0x000000FF;  //P2.0...P2.7 Output (LEDs on PORT2 defined as Output)
  
	/* LPC_GPIO2->FIOSET    = 0x000000FF;	//all LEDs on */
	LPC_GPIO2->FIOCLR    = 0x000000FF;  //all LEDs off
	
	//initialize led value
	led_value = 0;
	
	// At boot time, switch LD11 On, LD11 is the rightmost led on the board, connected to pin P2.0
	LED_On(0);
}

void LED_deinit(void) {

  LPC_GPIO2->FIODIR &= 0xFFFFFF00;  //P2.0...P2.7 Output LEDs on PORT2 defined as Output
}
