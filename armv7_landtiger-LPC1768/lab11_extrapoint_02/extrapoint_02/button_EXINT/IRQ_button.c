#include "button.h"
#include "lpc17xx.h"
#include "../timer/timer.h"
#include "../led/led.h"
#include "../RIT/RIT.h"		 

/*----------------------------------------------------------------------------
  BUTTON PRESSED COUNTER INCREMENTED BY RIT
 *----------------------------------------------------------------------------*/
extern int down;

/*----------------------------------------------------------------------------
  WHICH PUSHBUTTON HAS BEEN PRESSED
 *----------------------------------------------------------------------------*/
extern unsigned char PUSHBUTTON_TYPE;


/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;

/*----------------------------------------------------------------------------
	INT0: pushbutton for blind people
 *----------------------------------------------------------------------------*/
void EINT0_IRQHandler (void)	  	/* INT0	*/
{
	//disable interrupt and enable GPIO mode, it will be re-enabled by RIT
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	
	PUSHBUTTON_TYPE = BLIND_PUSHBUTTON;
	down = 1;	//button has been pressed
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt */
}


/*----------------------------------------------------------------------------
	KEY1: LEFT-SIDE PUSHBUTTON HANDLER
 *----------------------------------------------------------------------------*/
void EINT1_IRQHandler (void)	  	/* KEY1 */
{	
	//disable interrupt and enable GPIO mode, it will be re-enabled by RIT
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	
	PUSHBUTTON_TYPE = STANDARD_PUSHBUTTON;
	down = 1;	//button has been pressed
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt */
}


/*----------------------------------------------------------------------------
	KEY2: RIGHT-SIDE PUSHBUTTON HANDLER
 *----------------------------------------------------------------------------*/
void EINT2_IRQHandler (void)			/* KEY2 */
{	
	//disable interrupt and enable GPIO mode, it will be re-enabled by RIT
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	
	PUSHBUTTON_TYPE = STANDARD_PUSHBUTTON;
	down = 1;	//button has been pressed
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt */    
}


