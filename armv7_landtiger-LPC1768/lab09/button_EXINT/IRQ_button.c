#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"

// See LandTiger manual - Page 25, for buttons description

/*
* I redefine here a constant present in func_led.c, don't know why if I move it to led.h it gives me an error
*/
#define LED_NUM 8 	/* Number of user LEDs */
extern unsigned char led_value;	/* defined in funct_led	*/
extern unsigned char on_led; //led that is currently tuned on


/*
*	Handler for the interrupt generated for a key press of the INT0 button.
* The INT0 button is connected via the P2.10 pin
*
* When the INT0 button is pressed, status is resetted to the reset configuration:
* only LED11 (pin P2.0) is turned on
*/
void EINT0_IRQHandler (void)	  
{
	//turn off all leds
	int i;
	
	for(i=0; i<LED_NUM; i++){
		LED_Off(i);
	}
	
	//turn on LED11 (Pin 2.0)
	LED_On(0);
	on_led = 0;
  LPC_SC->EXTINT &= (1 << 0); /* clear pending interrupt, we need to clean the interrupt by hand, unless the bit stays set */
}

/*
*	Handler for the interrupt generated for a key press of the KEY1 button.
* The KEY1 button is connected via the P2.11 pin
*
* turn on the left led with respect to the current turned on led (on_led)
*/
void EINT1_IRQHandler (void)	  
{
	int new_on_led = (on_led+1)%(LED_NUM); //move left, if it's the last one the modulus will give zero, so start back from first
	LED_Off(on_led);
	LED_On(new_on_led);

	on_led = new_on_led;
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

/*
*	Handler for the interrupt generated for a key press of the KEY2 button.
* The KEY2 button is connected via the P2.12 pin
*
* turn on the right led with respect to the current turned on led (on_led)
*/
void EINT2_IRQHandler (void)	  
{
	int new_on_led;
	
	//move right, if it's the first one move to the last one
	if(on_led == 0)
		new_on_led = LED_NUM-1;
	else 
		new_on_led = (on_led-1)%(LED_NUM); 
	
	LED_Off(on_led);
	LED_On(new_on_led);

	on_led = new_on_led;
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


