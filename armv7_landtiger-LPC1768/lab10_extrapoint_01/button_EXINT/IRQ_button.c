#include "button.h"
#include "lpc17xx.h"
#include "../timer/timer.h"
#include "../led/led.h"

/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;

// do nothing
void EINT0_IRQHandler (void)	  	/* INT0	*/
{
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt */
}


/*----------------------------------------------------------------------------
	KEY1: LEFT-SIDE PUSHBUTTON HANDLER
 *----------------------------------------------------------------------------*/
void EINT1_IRQHandler (void)	  	/* KEY1 */
{
	BUTTON_pressed();
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt */
}


/*----------------------------------------------------------------------------
	KEY2: RIGHT-SIDE PUSHBUTTON HANDLER
 *----------------------------------------------------------------------------*/
void EINT2_IRQHandler (void)			/* KEY2 */
{
	BUTTON_pressed();
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt */    
}


