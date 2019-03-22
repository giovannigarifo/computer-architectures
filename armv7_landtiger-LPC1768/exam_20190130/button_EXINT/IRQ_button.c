#include "button.h"
#include "lpc17xx.h"
#include "../timer/timer.h"
#include "../led/led.h"
#include "../RIT/RIT.h"		 

/*----------------------------------------------------------------------------
  BUTTON PRESSED COUNTER INCREMENTED BY RIT
 *----------------------------------------------------------------------------*/
unsigned int down_INT0;
unsigned int down_KEY1;
unsigned int down_KEY2;


/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;

/*----------------------------------------------------------------------------
	INT0 handler
 *----------------------------------------------------------------------------*/
void EINT0_IRQHandler (void)	
{
	//disable interrupt and enable GPIO mode, it will be re-enabled by RIT
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	
	down_INT0 = 1;	//button has been pressed
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt */
}


/*----------------------------------------------------------------------------
	KEY1 handler
 *----------------------------------------------------------------------------*/
void EINT1_IRQHandler (void)	
{	
	//disable interrupt and enable GPIO mode, it will be re-enabled by RIT
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	
	down_KEY1 = 1;	//button has been pressed
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt */
}


/*----------------------------------------------------------------------------
	KEY2 handler
 *----------------------------------------------------------------------------*/
void EINT2_IRQHandler (void)		
{	
	//disable interrupt and enable GPIO mode, it will be re-enabled by RIT
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	
	down_KEY2 = 1;	//button has been pressed
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt */    
}


