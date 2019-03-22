#include "lpc17xx.h"
#include "adc.h"

/*----------------------------------------------------------------------------
  Function that initializes the ADC
 *----------------------------------------------------------------------------*/
void ADC_init (void) {

	// PIN mode of operation SELection:
  LPC_PINCON->PINSEL3 |=  (3UL<<30);      /* Set pin P1.31 to 11 so that is configured as input for the ADC AD0.5 */

  LPC_SC->PCONP       |=  (1<<12);      /* Enable power to ADC block */

  LPC_ADC->ADCR        =  (1<< 5) |     /* select AD0.5 pin as ADC input */
                          (4<< 8) |     /* ADC clock is 25MHz/5 */
                          (1<<21);      /* enable ADC */ 

  LPC_ADC->ADINTEN     =  (1<< 8);      /* global enable interrupt: interrupt mode enabled for each input */

  NVIC_EnableIRQ(ADC_IRQn);             /* enable ADC Interrupt */
}

/*----------------------------------------------------------------------------
  Start converting values from the potentiometer
 *----------------------------------------------------------------------------*/
void ADC_start_conversion (void) {
	LPC_ADC->ADCR |=  (1<<24);            /* Start A/D Conversion */
}				 

/*----------------------------------------------------------------------------
  Stop conversion of values from the input
 *----------------------------------------------------------------------------*/
void ADC_stop_conversion (void) {
	
	LPC_ADC->ADCR |=  (0<<24);            /* Stop A/D Conversion*/
}			

/*----------------------------------------------------------------------------
	Disable the ADC: from operational to powerdown mode
 *----------------------------------------------------------------------------*/
void ADC_disable (void) {
 	
	LPC_ADC->ADINTEN     =  (0<<8);			 /* disable global interrupt */
	
	NVIC_DisableIRQ(ADC_IRQn);           /* disable ADC Interrupt */
  
	LPC_ADC->ADCR       |= (0<<21);      /* enable power down mode */ 
}

/*----------------------------------------------------------------------------
	Enable the ADC: from power down to operational mode
 *----------------------------------------------------------------------------*/
void ADC_enable (void) {
 
	LPC_ADC->ADCR       |= (1<<21);      /* enable normal (power on) mode */ 

	LPC_ADC->ADINTEN     =  (1<<8);			 /* enable global interrupt */
	
	NVIC_EnableIRQ(ADC_IRQn);            /* enable ADC Interrupt */
}
